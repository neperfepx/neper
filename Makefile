# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line.
SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
SPHINXPROJ    = ReadtheDocsSphinxTheme
SOURCEDIR     = .
BUILDDIR      = build

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

html:
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
	cp -rp imgs _static/* $(BUILDDIR)/html
	rm -rf docs
	mv $(BUILDDIR)/html docs
	touch docs/.nojekyll
	rm -rf $(BUILDDIR)

clean:
	rm -rf docs
