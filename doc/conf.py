# -*- coding: utf-8 -*-

import sys
import os
import re

# Prefer to use the version of the theme in this repo
# and not the installed version of the theme.
sys.path.insert(0, os.path.abspath('.'))

import sphinx_rtd_theme

project = u'Neper'
version = u'4.4.2-33'
release = u'4.4.2-33'
author = u'Romain Quey'
copyright = u'Romain Quey'
language = 'en'

html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'logo_only': False,
    'navigation_depth': 5,
}
