# -*- coding: utf-8 -*-

# Sphinx configuration of Neper's user documentation.
#
# Build:  make html
# Needs:  Sphinx and, for the look of the website, pydata-sphinx-theme; without the theme, the
#         built-in "alabaster" theme is used.  Same theme and settings as FEPX's documentation and the
#         websites (https://neper.info, https://fepx.info).

import sys
import os
import re

# Archives of the simulation directories of the tutorials, for download (sim_archives.py)
sys.path.insert(0, os.path.abspath('.'))

project = u'Neper'
version = u'7.0.0'
release = u'7.0.0'
author = u'Romain Quey'
copyright = u'Romain Quey, CNRS'
language = 'en'

extensions = ['sim_archives']

# Theme: the website's (pydata-sphinx-theme, light mode only) when installed, otherwise
# Sphinx's default.
try:
    import pydata_sphinx_theme  # noqa: F401
    html_theme = 'pydata_sphinx_theme'
    html_logo = '_static/neper-logo-text.svg'
    html_theme_options = {
        'logo': {'alt_text': 'Neper'},
        'navbar_align': 'left',
        'navbar_center': [],
        'navbar_end': [],
        'show_nav_level': 1,
        'navigation_depth': 5,
        'show_toc_level': 2,
        'secondary_sidebar_items': ['page-toc'],
        'navbar_start': ['navbar-logo', 'neper-version'],
        'footer_start': ['copyright'],
        'footer_end': ['theme-version'],
    }
    html_context = {'default_mode': 'light'}
    # Every page of the documentation is a top-level page, which the theme would list in the
    # header and leave the section navigation empty: list them in the left sidebar instead, as
    # on the website (where they are under "Documentation").
    html_sidebars = {'**': ['neper-doc-nav.html']}
    templates_path = ['_templates']
except ImportError:
    html_theme = 'alabaster'

html_favicon = '_static/neper-favicon.png'
html_static_path = ['_static']
html_css_files = ['custom.css']
html_title = 'Neper documentation'


def setup(app):
    # Light mode only, whatever a previous visit saved in the browser
    app.add_js_file(None, body="""
      try {
        localStorage.setItem("mode", "light");
        localStorage.setItem("theme", "light");
      } catch (e) {}
      document.documentElement.dataset.mode = "light";
      document.documentElement.dataset.theme = "light";
    """)
