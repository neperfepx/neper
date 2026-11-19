// Chart of the application papers per year and field (applications.rst), drawn from
// the list itself and redrawn for the papers that the search box and filters show.
// It goes into the filter box (<div id="neper-app-filter">), after the paper count.
document.addEventListener('DOMContentLoaded', function () {
  var box = document.getElementById('neper-app-filter');
  if (!box) return;

  var NCOLORS = 8;   // fields with their own colour; the others are merged into "other"
  var COLORS = ['#2a78d6', '#eb6834', '#1baf7a', '#eda100', '#e87ba4', '#008300', '#6250d6', '#e34948'];
  var GREY = '#c9c8c0';
  var NOW = new Date().getFullYear();   // this year and later: papers still being indexed

  // Papers: year, fields
  var papers = [];
  Array.prototype.forEach.call(document.querySelectorAll('section'), function (s) {
    var h = s.querySelector(':scope > h2');
    if (!h) return;
    var y = h.textContent.replace('#', '').trim();
    if (!/^\d{4}$/.test(y)) return;
    s.querySelectorAll(':scope > ul > li').forEach(function (li) {
      var tags = Array.prototype.map.call(li.querySelectorAll('.neper-tag'), function (t) { return t.dataset.tag; });
      papers.push({ li: li, section: s, year: +y, tags: tags.length ? tags : ['other'] });
    });
  });
  if (!papers.length) return;

  // Fields, the largest first (a paper with n fields counts 1/n in each); colours are
  // fixed once from all the papers so that a field keeps its colour when filtering
  var labels = { other: 'other' }, weight = {};
  papers.forEach(function (p) {
    p.tags.forEach(function (k) { weight[k] = (weight[k] || 0) + 1 / p.tags.length; });
  });
  document.querySelectorAll('li .neper-tag').forEach(function (t) { labels[t.dataset.tag] = t.textContent; });
  var named = Object.keys(weight).filter(function (k) { return k !== 'other'; })
    .sort(function (a, b) { return weight[b] - weight[a]; });
  var kept = named.slice(0, NCOLORS), merged = named.slice(NCOLORS);
  var series = kept.map(function (k, i) { return { keys: [k], name: labels[k], color: COLORS[i] }; });
  var otherKeys = ['other'].concat(merged);
  series.push({ keys: otherKeys, color: GREY,
                name: 'other' + (merged.length ? ' (incl. ' + merged.map(function (k) { return labels[k]; }).join(', ') + ')' : '') });
  var seriesOf = {};
  series.forEach(function (s, i) { s.keys.forEach(function (k) { seriesOf[k] = i; }); });

  var years = papers.map(function (p) { return p.year; });
  var y0 = Math.min.apply(null, years), y1 = Math.max.apply(null, years);

  // Elements
  var wrap = document.createElement('details');
  wrap.className = 'neper-app-chart';
  wrap.innerHTML =
    '<summary>Papers per year, by field</summary>' +
    '<div class="neper-app-chart-head">' +
    '<div class="neper-app-chart-switch" role="group" aria-label="Show">' +
    '<button type="button" aria-pressed="true">Papers</button>' +
    '<button type="button" aria-pressed="false">Share (%)</button></div>' +
    '<span class="neper-app-chart-note"></span></div>' +
    '<div class="neper-app-chart-legend"></div>' +
    '<div class="neper-app-chart-plot"></div>' +
    '<p class="neper-app-chart-foot">A paper with several fields counts as an equal fraction in each. ' +
    'Fields are assigned from keywords and are approximate. Lighter bars: papers of ' + NOW +
    ' and later are still being indexed.</p>';
  var count = box.querySelector('.neper-app-count');
  box.insertBefore(wrap, count ? count.nextSibling : null);
  var btns = wrap.querySelectorAll('.neper-app-chart-switch button');
  var plot = wrap.querySelector('.neper-app-chart-plot');
  var note = wrap.querySelector('.neper-app-chart-note');
  wrap.querySelector('.neper-app-chart-legend').innerHTML = series.map(function (s) {
    return '<span><i style="background:' + s.color + '"></i>' + s.name + '</span>';
  }).join('');
  var share = false;
  btns[0].addEventListener('click', function () { share = false; draw(); });
  btns[1].addEventListener('click', function () { share = true; draw(); });
  wrap.addEventListener('toggle', draw);

  var NS = 'http://www.w3.org/2000/svg';
  function el(n, a, p) {
    var e = document.createElementNS(NS, n);
    for (var k in a) e.setAttribute(k, a[k]);
    if (p) p.appendChild(e);
    return e;
  }
  function r1(v) { return Math.round(v * 10) / 10; }

  function rows() {
    var byYear = {};
    for (var y = y0; y <= y1; y++) byYear[y] = { label: String(y), year: y, n: 0, v: series.map(function () { return 0; }) };
    papers.forEach(function (p) {
      if (p.li.hidden || p.section.hidden) return;
      var r = byYear[p.year];
      r.n++;
      p.tags.forEach(function (k) { r.v[seriesOf[k]] += 1 / p.tags.length; });
    });
    return Object.keys(byYear).map(function (y) { return byYear[y]; });
  }

  function draw() {
    btns[0].setAttribute('aria-pressed', String(!share));
    btns[1].setAttribute('aria-pressed', String(share));
    if (!wrap.open) return;
    var data = rows();
    var total = data.reduce(function (s, d) { return s + d.n; }, 0);
    note.textContent = total === papers.length ? total + ' papers' : total + ' of ' + papers.length + ' papers (filtered)';
    plot.innerHTML = '';
    if (!total) { plot.textContent = 'No papers match the search.'; return; }

    var W = 900, H = 300, m = { l: 42, r: 8, t: 18, b: 24 };
    var svg = el('svg', { viewBox: '0 0 ' + W + ' ' + H, role: 'img',
                          'aria-label': share ? 'Share of each field per year' : 'Papers per year, by field' }, plot);
    var iw = W - m.l - m.r, ih = H - m.t - m.b;
    var max = Math.max.apply(null, data.map(function (d) { return d.n; }));
    var step = share ? 0.25 : [1, 2, 5, 10, 20, 25, 50, 100].filter(function (s) { return max / s <= 6; })[0] || 100;
    var top = share ? 1 : Math.ceil(max / step) * step;
    var Y = function (v) { return m.t + ih - v / top * ih; };
    for (var v = 0; v <= top + 1e-9; v += step) {
      el('line', { x1: m.l, x2: W - m.r, y1: Y(v), y2: Y(v), class: 'grid' }, svg);
      el('text', { x: m.l - 6, y: Y(v) + 4, 'text-anchor': 'end' }, svg).textContent = share ? Math.round(v * 100) + '%' : v;
    }
    var bw = iw / data.length, w = Math.min(bw * 0.72, 30), every = data.length > 24 ? 3 : data.length > 12 ? 2 : 1;
    var tip = document.createElement('div');
    tip.className = 'neper-app-chart-tip';
    tip.hidden = true;
    plot.appendChild(tip);

    data.forEach(function (d, i) {
      var cx = m.l + i * bw + bw / 2;
      if (i % every === 0 || d.label.length > 4)
        el('text', { x: cx, y: H - 6, 'text-anchor': 'middle' }, svg).textContent = d.label;
      var acc = 0, den = share ? (d.n || 1) : 1, op = d.year >= NOW ? 0.4 : 1;
      var segs = [];
      d.v.forEach(function (val, j) { if (val > 0) segs.push(j); });
      segs.forEach(function (j, k) {
        var a = Y(acc), b = Y(acc + d.v[j] / den), h = Math.max(0, a - b - (k ? 2 : 0));
        var x = cx - w / 2;
        if (k === segs.length - 1) {
          var r = Math.min(3, h / 2, w / 2);
          el('path', { d: 'M' + x + ',' + (b + h) + 'V' + (b + r) + 'Q' + x + ',' + b + ' ' + (x + r) + ',' + b +
                          'H' + (x + w - r) + 'Q' + (x + w) + ',' + b + ' ' + (x + w) + ',' + (b + r) + 'V' + (b + h) + 'Z',
                       fill: series[j].color, 'fill-opacity': op }, svg);
        } else {
          el('rect', { x: x, y: b, width: w, height: h, fill: series[j].color, 'fill-opacity': op }, svg);
        }
        acc += d.v[j] / den;
      });
      if (d.n && (share || data.length <= 12 || d.n >= max / 8))
        el('text', { x: cx, y: (share ? m.t : Y(d.n)) - 5, 'text-anchor': 'middle', class: 'val' }, svg).textContent = d.n;
      var hit = el('rect', { x: m.l + i * bw, y: 0, width: bw, height: m.t + ih, fill: 'transparent' }, svg);
      hit.addEventListener('pointerenter', function () {
        tip.innerHTML = '<b>' + d.label + '</b> · ' + d.n + ' paper' + (d.n === 1 ? '' : 's') + (d.year >= NOW ? ' (still being indexed)' : '') +
          series.map(function (s, j) {
            return d.v[j] ? '<div><span><i style="background:' + s.color + '"></i>' + s.name + '</span><span>' +
              (share ? Math.round(d.v[j] / d.n * 100) + '%' : r1(d.v[j])) + '</span></div>' : '';
          }).reverse().join('');
        tip.hidden = false;
        var pr = plot.getBoundingClientRect(), sr = svg.getBoundingClientRect();
        var px = cx / W * sr.width + sr.left - pr.left, tw = tip.offsetWidth;
        tip.style.left = Math.max(0, px + 12 + tw > pr.width ? px - tw - 12 : px + 12) + 'px';
      });
      hit.addEventListener('pointerleave', function () { tip.hidden = true; });
    });
  }

  // Redraw when the search box or a filter changes the list
  new MutationObserver(function (ms) {
    if (!ms.some(function (m) { return m.target.tagName === 'LI' || m.target.tagName === 'SECTION'; })) return;
    clearTimeout(draw.t); draw.t = setTimeout(draw, 50);
  })
    .observe(document.querySelector('article') || document.body, { attributes: true, attributeFilter: ['hidden'], subtree: true });
  draw();
});
