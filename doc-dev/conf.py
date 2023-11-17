# -*- coding: utf-8 -*-

import sys
import os
import re

# Prefer to use the version of the theme in this repo
# and not the installed version of the theme.
sys.path.insert(0, os.path.abspath('.'))

import sphinx_rtd_theme

project = u'Neper'
version = u'4.6.1-8-devel'
release = u'4.6.1-8-devel'
author = u'Romain Quey'
copyright = u'Romain Quey'
language = 'en'

html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'logo_only': False,
    'navigation_depth': 5,
}

def setup(app):
    app.add_css_file('my_theme.css')

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
