# -*- coding: utf-8 -*-

import sys
import os
import re

extensions = ['sphinx_design', 'sphinx_copybutton', 'sphinxext.opengraph']

# Archives of the tutorials' simulation directories, written by the extension of the copied
# documentation (doc/sim_archives.py), when it provides it
if os.path.exists(os.path.join('doc', 'sim_archives.py')):
    sys.path.insert(0, os.path.abspath('doc'))
    extensions.append('sim_archives')

# Copy commands without the prompt, and keep the lines of multi-line commands
copybutton_prompt_text = '$ '
copybutton_line_continuation_character = '\\'

ogp_site_url = 'https://neper.info/'
ogp_site_name = 'Neper'
ogp_image = 'https://neper.info/imgs/home-neper.png'
ogp_social_cards = {'enable': False}

project = u'Neper'
version = u'7.0.0'
release = version
author = u'Romain Quey'
copyright = u'Romain Quey, CNRS'
language = 'en'

templates_path = ['_templates']

html_theme = 'pydata_sphinx_theme'
html_title = 'Neper'
html_logo = '_static/neper-logo-text.svg'
html_favicon = '_static/neper-favicon.png'
html_theme_options = {
    'logo': {'alt_text': 'Neper'},
    'navbar_align': 'left',
    'navbar_end': [],
    'header_links_before_dropdown': 8,
    'show_nav_level': 1,
    'navigation_depth': 5,
    'show_toc_level': 2,
    'secondary_sidebar_items': ['page-toc'],
    'navbar_start': ['navbar-logo', 'neper-version'],
    'footer_start': ['copyright'],
    'footer_end': ['theme-version'],
}

# Light mode only: the images have white backgrounds
html_context = {'default_mode': 'light'}

# These pages have no section navigation to show
html_sidebars = {page: [] for page in ('index', 'gallery', 'papers', 'applications',
                                       'downloads', 'whatsnew', 'community')}

def setup(app):
    app.add_css_file('my_theme.css')
    # Light mode only, whatever a previous visit saved in the browser
    app.add_js_file(None, body="""
      try {
        localStorage.setItem("mode", "light");
        localStorage.setItem("theme", "light");
      } catch (e) {}
      document.documentElement.dataset.mode = "light";
      document.documentElement.dataset.theme = "light";
    """)

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
