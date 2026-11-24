// What's New: show the changes between two versions, selected by the reader.
//
// The versions are listed in the data-versions attribute of the selector paragraph (oldest
// first).  By default, the new version is the latest one, X.Y.Z, and the old version (reference)
// is the previous major or minor version, i.e. the latest X'.Y'.0 version with X'.Y' < X.Y (not
// a patch version).  The items of the page are the sections that contain text (e.g., highlights) and,
// in the sections that contain only lists, the list items and admonitions.  An item is shown
// when the conditions of its tags hold, where the tags are written in the reStructuredText
// source using roles (see conf.py):
//
// - :since:`X`: item introduced in version X (old < X <= new), shown as a badge when several
//   releases are compared.  Items without :since: are introduced in the first version after
//   the oldest one.
// - :when:`cond[,cond...]`: additional conditions, of the form old|new <op> X, where <op> is
//   <, <=, >, >= or ==, for example to describe a change differently depending on whether the
//   intermediate releases are included.
//
// Other content (e.g., a paragraph of the introduction) is shown always, or, when it holds a
// tag, when the conditions of its tags hold (with no default :since:).
//
// A section whose items are all hidden is hidden too, as well as its entry in the page table
// of contents.  The selection is kept in the URL (?from=X&to=Y).

(function () {
  'use strict';

  function cmp(a, b) {
    var x = a.split('.').map(Number), y = b.split('.').map(Number);
    for (var i = 0; i < Math.max(x.length, y.length); i++) {
      var d = (x[i] || 0) - (y[i] || 0);
      if (d) return d < 0 ? -1 : 1;
    }
    return 0;
  }

  function test(cond, old, nw) {
    var m = cond.trim().match(/^(old|new)\s*(<=|>=|==|<|>)\s*([0-9.]+)$/);
    if (!m) {
      console.warn('whatsnew: invalid condition "' + cond + '"');
      return true;
    }
    var c = cmp(m[1] === 'old' ? old : nw, m[3]);
    return { '<': c < 0, '<=': c <= 0, '>': c > 0, '>=': c >= 0, '==': c === 0 }[m[2]];
  }

  function isItemContainer(sec) {
    // A section that contains only its title, lists, admonitions, targets and subsections
    return Array.prototype.every.call(sec.children, function (el) {
      return /^H[1-6]$/.test(el.tagName) || el.tagName === 'UL' || el.tagName === 'SECTION'
        || el.tagName === 'SPAN' || el.classList.contains('admonition');
    });
  }

  document.addEventListener('DOMContentLoaded', function () {
    var ctrl = document.querySelector('.neper-whatsnew-versions');
    if (!ctrl) return;
    var page = ctrl.closest('section');
    var versions = ctrl.dataset.versions.trim().split(/\s+/);
    var oldSel = ctrl.querySelector('select[name="from"]');
    var newSel = ctrl.querySelector('select[name="to"]');

    // Items, with their conditions
    var items = [];
    function collect(sec) {
      Array.prototype.forEach.call(sec.children, function (el) {
        if (el.tagName === 'SECTION') {
          if (isItemContainer(el)) collect(el);
          else items.push(el);
        } else if (el.tagName === 'UL') {
          Array.prototype.forEach.call(el.children, function (li) { items.push(li); });
        } else if (el.classList.contains('admonition')) {
          items.push(el);
        } else if (el.querySelector('.neper-since, .neper-when')) {
          el.neperExplicit = true;
          items.push(el);
        }
      });
    }
    collect(page);

    items.forEach(function (item) {
      var since = item.neperExplicit ? null : versions[1], conds = [];
      item.querySelectorAll('.neper-since, .neper-when').forEach(function (tag) {
        // Only the tags of this item, not those of nested items
        var owner = tag.parentElement;
        while (owner && items.indexOf(owner) === -1) owner = owner.parentElement;
        if (owner !== item) return;
        if (tag.classList.contains('neper-since')) since = tag.textContent.trim();
        else conds = conds.concat(tag.textContent.split(','));
      });
      item.neperConds = (since ? ['old<' + since, 'new>=' + since] : []).concat(conds);
    });

    var sections = Array.prototype.slice.call(page.querySelectorAll('section')).reverse();
    var tocLinks = document.querySelectorAll('.page-toc a.nav-link[href^="#"]');

    function update(push) {
      var old = oldSel.value, nw = newSel.value;
      items.forEach(function (item) {
        var show = item.neperConds.every(function (c) { return test(c, old, nw); });
        item.classList.toggle('neper-hidden', !show);
      });

      // Sections whose items are all hidden (deepest first), starting from the state of the
      // items only (not from that of the previous selection)
      page.querySelectorAll('section, ul').forEach(function (el) {
        if (items.indexOf(el) === -1) el.classList.remove('neper-hidden');
      });
      sections.forEach(function (sec) {
        if (items.indexOf(sec) !== -1) return;
        var children = Array.prototype.filter.call(sec.querySelectorAll('li, .admonition, section'),
          function (el) { return items.indexOf(el) !== -1 || el.tagName === 'SECTION'; });
        var empty = children.length > 0 && children.every(function (el) {
          return el.closest('.neper-hidden') !== null;
        });
        sec.classList.toggle('neper-hidden', empty);
        // Lists whose items are all hidden
        sec.querySelectorAll(':scope > ul').forEach(function (ul) {
          var all = Array.prototype.every.call(ul.children, function (li) {
            return li.classList.contains('neper-hidden');
          });
          ul.classList.toggle('neper-hidden', all);
        });
      });

      tocLinks.forEach(function (a) {
        var target = document.getElementById(decodeURIComponent(a.getAttribute('href').slice(1)));
        if (target) a.parentElement.classList.toggle('neper-hidden',
                                                     target.closest('.neper-hidden') !== null);
      });

      // Badges, when several releases are compared
      var nb = versions.filter(function (v) { return cmp(v, old) > 0 && cmp(v, nw) <= 0; }).length;
      page.classList.toggle('neper-whatsnew-multi', nb > 1);

      if (push) {
        var url = new URL(window.location.href);
        url.searchParams.set('from', old);
        url.searchParams.set('to', nw);
        history.replaceState(null, '', url);
      }
    }

    function fill(sel, list, value) {
      sel.innerHTML = '';
      list.slice().reverse().forEach(function (v) {
        var o = document.createElement('option');
        o.value = o.textContent = v;
        sel.appendChild(o);
      });
      sel.value = value;
    }

    // Default versions: the latest one, and the previous major or minor version
    var latest = versions[versions.length - 1];
    var minor = latest.replace(/\.[^.]*$/, '.0');
    var ref = versions.filter(function (v) {
      return /\.0$/.test(v) && cmp(v, minor) < 0;
    }).pop() || versions[0];

    var params = new URLSearchParams(window.location.search);
    var nw = params.get('to'), old = params.get('from');
    if (versions.indexOf(nw) < 1) nw = latest;
    if (versions.indexOf(old) < 0 || cmp(old, nw) >= 0)
      old = cmp(ref, nw) < 0 ? ref : versions[versions.indexOf(nw) - 1];
    fill(oldSel, versions.slice(0, -1), old);
    fill(newSel, versions.slice(1), nw);

    oldSel.addEventListener('change', function () {
      if (cmp(oldSel.value, newSel.value) >= 0)
        newSel.value = versions[versions.indexOf(oldSel.value) + 1];
      update(true);
    });
    newSel.addEventListener('change', function () {
      if (cmp(oldSel.value, newSel.value) >= 0)
        oldSel.value = versions[versions.indexOf(newSel.value) - 1];
      update(true);
    });

    update(false);
  });
})();
