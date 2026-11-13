# Neper website

Sources of https://neper.info.

- `website-devel`: working branch, pushed to the private repository neperfepx/neper-dev.
  Each push builds a preview (with the documentation of Neper's `devel` branch), available
  as the `website-preview` artifact of the workflow run (see "Viewing the preview").
- `website`: published branch (squash-merged from `website-devel`).  Each push builds the
  website (with the documentation of Neper's `main` branch) and deploys it.

The documentation (`doc/`) is not stored here: `make html` copies it from a Neper branch
(committed files only) before building.

## Building locally

```
pip install -r requirements.txt
make html          # copies doc/ from ../neper at devel, then builds build/html/
```

Then open `build/html/index.html` (later builds only rebuild the pages that changed).  The Neper repository and branch can be changed, as in
`make html NEPER_REPO=<path> DOC_REF=main`; `make doc` only copies the documentation.

## Viewing the preview

neperfepx/neper-dev is private and cannot host the preview.  To download the latest
preview and serve it at http://localhost:8000:

```
./preview.sh [<port>]
```

The first run downloads the preview (about 90 MB); later runs reuse it until a new preview
is built.  Previews expire after 7 days; to rebuild one:

```
gh workflow run website.yaml --repo neperfepx/neper-dev --ref website-devel
```

## Large files

Old Neper versions are not stored here: they are files of the
`website-assets` release of neperfepx/neper (see `upload-assets.sh`).
