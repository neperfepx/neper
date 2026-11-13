# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line.
SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
SPHINXPROJ    = ReadtheDocsSphinxTheme
SOURCEDIR     = .
BUILDDIR      = build

# Neper repository and branch (or tag, commit) from which the documentation is copied.
# Only committed files are copied.  The workflow overrides them (main for the website).
NEPER_REPO    ?= ../neper
DOC_REF       ?= devel

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile html doc clean

# Copy doc/ and VERSIONS from $(NEPER_REPO) at $(DOC_REF), keeping the website's doc/conf.py
doc:
	@git -C "$(NEPER_REPO)" rev-parse --verify --quiet "$(DOC_REF)^{commit}" > /dev/null 2>&1 \
	  || { echo "No commit $(DOC_REF) in Neper repository $(NEPER_REPO) (set NEPER_REPO=<path>)"; exit 1; }
	@rm -rf $(BUILDDIR)/doc-src && mkdir -p $(BUILDDIR)/doc-src
	@git -C "$(NEPER_REPO)" archive "$(DOC_REF)" -- ":(top)doc" ":(top)VERSIONS" | tar -x -C $(BUILDDIR)/doc-src
	@rsync -a --delete --exclude=/conf.py $(BUILDDIR)/doc-src/doc/ doc/
	@cp $(BUILDDIR)/doc-src/VERSIONS VERSIONS
	@sed -i 's/^Neper: Polycrystal Generation.*$$/Documentation/' doc/index.rst
	@rm -rf $(BUILDDIR)/doc-src
	@echo "Copied doc/ and VERSIONS from $(DOC_REF) ($$(git -C "$(NEPER_REPO)" rev-parse --short "$(DOC_REF)"))"

html: doc
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
	cp -rp imgs _static/* $(BUILDDIR)/html
	@echo "The website is in $(BUILDDIR)/html."

clean:
	rm -rf $(BUILDDIR)
