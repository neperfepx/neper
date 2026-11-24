# -*- coding: utf-8 -*-

import sys
import os
import re

from docutils import nodes

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
version = u'7.0.1'
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

# What's New tags (see _static/whatsnew.js): :since:`<version>` (version that introduced an
# item, shown as a badge) and :when:`<conditions>` (additional conditions, not shown)
def whatsnew_role(name, rawtext, text, lineno, inliner, options=None, content=None):
    return [nodes.inline(rawtext, text, classes=['neper-' + name])], []

# Field tags of the application papers (applications.rst), written as :tag:`key`
APPLICATION_TAGS = {
    'cp': 'crystal plasticity',
    'frac': 'fracture & fatigue',
    'rock': 'rocks & granular media',
    'proc': 'processing & microstructure evolution',
    'phys': 'physical & functional properties',
    'char': 'characterization',
    'am': 'additive manufacturing',
    'waves': 'waves & ultrasonics',
    'meth': 'methods & data',
    'other': 'other',
}

def tag_role(name, rawtext, text, lineno, inliner, options=None, content=None):
    from docutils import nodes
    key = text.strip()
    if key not in APPLICATION_TAGS:
        msg = inliner.reporter.error('Unknown application tag "%s" (see APPLICATION_TAGS in conf.py)'
                                     % key, line=lineno)
        return [inliner.problematic(rawtext, rawtext, msg)], [msg]
    html = '<span class="neper-tag" data-tag="%s">%s</span>' % (key, APPLICATION_TAGS[key])
    return [nodes.raw('', html, format='html')], []

def setup(app):
    app.add_role('tag', tag_role)
    app.add_js_file('applications.js')
    app.add_js_file('applications-chart.js')
    app.add_css_file('my_theme.css')
    app.add_role('since', whatsnew_role)
    app.add_role('when', whatsnew_role)
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
