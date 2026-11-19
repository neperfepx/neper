// Search box, field filter and journal filter of the application papers (applications.rst).
// Several fields or journals can be selected: a paper is shown if it has any of the
// selected fields and is in any of the selected journals.
document.addEventListener('DOMContentLoaded', function () {
  var box = document.getElementById('neper-app-filter');
  if (!box) return;

  // Journals with at least this number of papers get a filter button
  // (<div id="neper-app-filter" data-min-journal-papers="N">, 10 by default)
  var MIN_JOURNAL_PAPERS = parseInt(box.dataset.minJournalPapers, 10) || 10;

  var norm = function (s) {
    return s.normalize('NFD').replace(/[̀-ͯ]/g, '').toLowerCase();
  };

  // The year sections and their papers
  var sections = Array.prototype.filter.call(document.querySelectorAll('section'), function (s) {
    var h = s.querySelector(':scope > h2');
    return h && /^\s*\d{4}\s*$/.test(h.textContent.replace('#', ''));
  });
  var papers = [];
  sections.forEach(function (s) {
    s.querySelectorAll(':scope > ul > li').forEach(function (li) {
      var tags = Array.prototype.map.call(li.querySelectorAll('.neper-tag'), function (t) {
        return t.dataset.tag;
      });
      var strong = li.querySelector('strong');
      papers.push({ li: li, section: s, text: norm(li.textContent), tags: tags,
                    journal: strong ? strong.textContent.trim() : '' });
    });
  });

  // Fields (tags): labels and counts, the largest first and "other" last
  var labels = {}, counts = {}, fields = [];
  papers.forEach(function (p) {
    p.li.querySelectorAll('.neper-tag').forEach(function (t) {
      var k = t.dataset.tag;
      if (!(k in labels)) { labels[k] = t.textContent; counts[k] = 0; fields.push(k); }
      counts[k]++;
    });
  });
  fields.sort(function (a, b) {
    return a === 'other' ? 1 : b === 'other' ? -1 : counts[b] - counts[a];
  });

  // Main journals, the largest first
  var jcounts = {};
  papers.forEach(function (p) { if (p.journal) jcounts[p.journal] = (jcounts[p.journal] || 0) + 1; });
  var journals = Object.keys(jcounts).filter(function (j) { return jcounts[j] >= MIN_JOURNAL_PAPERS; });
  journals.sort(function (a, b) { return jcounts[b] - jcounts[a] || a.localeCompare(b); });
  // All other journals (and entries without a journal) under one button
  var OTHERS = '\u0000others';
  var main = {};
  journals.forEach(function (j) { main[j] = true; });
  var nothers = papers.filter(function (p) { return !main[p.journal]; }).length;
  if (nothers) journals.push(OTHERS);

  var input = document.createElement('input');
  input.type = 'search';
  input.placeholder = 'Search authors, titles, journals…';
  input.setAttribute('aria-label', 'Search the application papers');
  input.className = 'neper-app-search';
  box.appendChild(input);
  var hint = document.createElement('p');
  hint.className = 'neper-app-hint';
  hint.innerHTML = 'Several words: papers with all of them. <code>OR</code> for either, ' +
    '<code>-word</code> to exclude, <code>"exact phrase"</code>.';
  box.appendChild(hint);

  // Filters: the selected fields and journals (none selected: no filtering)
  var active = { field: {}, journal: {} };
  var buttons = { field: {}, journal: {} };

  function row(kind, title, keys, label, count) {
    var div = document.createElement('div');
    div.className = 'neper-app-chips';
    var name = document.createElement('span');
    name.className = 'neper-app-chips-title';
    name.textContent = title;
    div.appendChild(name);
    keys.forEach(function (k) {
      var b = document.createElement('button');
      b.type = 'button';
      b.setAttribute('aria-pressed', 'false');
      b.className = 'neper-tag' + (kind === 'journal' && k !== OTHERS ? ' neper-journal' : '');
      b.textContent = label(k) + ' (' + count(k) + ')';
      b.addEventListener('click', function () { toggle(kind, k); });
      div.appendChild(b);
      buttons[kind][k] = b;
    });
    box.appendChild(div);
  }
  row('field', 'Fields', fields, function (k) { return labels[k]; }, function (k) { return counts[k]; });
  row('journal', 'Journals', journals,
      function (j) { return j === OTHERS ? 'other' : j; },
      function (j) { return j === OTHERS ? nothers : jcounts[j]; });

  var count = document.createElement('p');
  count.className = 'neper-app-count';
  box.appendChild(count);
  var clear = document.createElement('button');
  clear.type = 'button';
  clear.className = 'neper-app-clear';
  clear.textContent = 'Clear filters';
  clear.hidden = true;
  clear.addEventListener('click', function () {
    active = { field: {}, journal: {} };
    ['field', 'journal'].forEach(function (kind) {
      Object.keys(buttons[kind]).forEach(function (o) {
        buttons[kind][o].classList.remove('active');
        buttons[kind][o].setAttribute('aria-pressed', 'false');
      });
    });
    clear.hidden = true;
    apply();
  });
  count.appendChild(clear);

  function toggle(kind, k) {
    if (active[kind][k]) delete active[kind][k]; else active[kind][k] = true;
    Object.keys(buttons[kind]).forEach(function (o) {
      buttons[kind][o].classList.toggle('active', !!active[kind][o]);
      buttons[kind][o].setAttribute('aria-pressed', String(!!active[kind][o]));
    });
    clear.hidden = !Object.keys(active.field).length && !Object.keys(active.journal).length;
    apply();
  }

  // A tag in a paper entry also filters
  papers.forEach(function (p) {
    p.li.querySelectorAll('.neper-tag').forEach(function (t) {
      t.addEventListener('click', function () {
        toggle('field', t.dataset.tag);
        box.scrollIntoView({ behavior: 'smooth' });
      });
    });
  });

  // A query: terms ("quoted phrases" or single words) that must all match (AND, or just
  // spaces), alternatives separated by OR, and terms to exclude written -term or -"phrase".
  // AND and OR are operators only in capitals.  Returns a list of alternatives, each a
  // list of {t: normalized text, not: true|false}, and whether operators were used.
  function parse(raw) {
    var alts = [[]], ops = false, re = /(-?)"([^"]*)"?|(\S+)/g, m;
    while ((m = re.exec(raw))) {
      var word = m[3];
      if (word === 'OR') { ops = true; if (alts[alts.length - 1].length) alts.push([]); continue; }
      if (word === 'AND') { ops = true; continue; }
      var not = !!m[1], t = m[2] !== undefined ? m[2] : word;
      if (m[2] !== undefined) ops = true;
      if (word && word.length > 1 && word[0] === '-') { not = true; t = word.slice(1); }
      if (not) ops = true;
      t = norm(t).replace(/\s+/g, ' ').trim();
      if (t) alts[alts.length - 1].push({ t: t, not: not });
    }
    alts = alts.filter(function (a) { return a.length; });
    return { alts: alts, ops: ops };
  }

  function apply() {
    var q = norm(input.value).replace(/\s+/g, ' ').trim();
    var fields = Object.keys(active.field), jsel = Object.keys(active.journal);
    var inFilters = function (p) {
      return (!fields.length || fields.some(function (k) { return p.tags.indexOf(k) >= 0; })) &&
             (!jsel.length || jsel.some(function (j) {
               return p.journal === j || (j === OTHERS && !main[p.journal]);
             }));
    };
    var has = function (p, t) { return p.text.indexOf(t) >= 0; };
    var query = parse(input.value);
    var match = function (p) {
      return !query.alts.length || query.alts.some(function (a) {
        return a.every(function (w) { return has(p, w.t) !== w.not; });
      });
    };
    // Without operators, a query of several words matches as a phrase first ("international
    // journal of plasticity"), and as separate words when the phrase is found nowhere
    // ("huang ultrasonics")
    if (!query.ops && query.alts.length === 1 && query.alts[0].length > 1 &&
        papers.some(function (p) { return inFilters(p) && has(p, q); })) {
      match = function (p) { return has(p, q); };
    }
    var shown = 0, perSection = new Map();
    papers.forEach(function (p) {
      var ok = inFilters(p) && match(p);
      p.li.hidden = !ok;
      if (ok) { shown++; perSection.set(p.section, true); }
    });
    sections.forEach(function (s) { s.hidden = !perSection.get(s); });
    count.firstChild && count.firstChild.nodeType === 3
      ? count.firstChild.textContent = msg(shown)
      : count.insertBefore(document.createTextNode(msg(shown)), count.firstChild);
  }

  function msg(shown) {
    return (shown === papers.length ? papers.length : shown + ' of ' + papers.length) + ' papers ';
  }

  input.addEventListener('input', apply);
  apply();
});
