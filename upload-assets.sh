#!/bin/bash
# Upload the large files of the website (old Neper tarballs) to the
# "website-assets" release of neperfepx/neper, which the website links to.
#
# Usage: ./upload-assets.sh <assets_directory>
#
# The release is marked as a pre-release, so that it is not shown as the latest Neper release.
# Existing files of the same names are replaced (--clobber).

set -euo pipefail

dir=${1:?usage: $0 <assets_directory>}
repo=neperfepx/neper
tag=website-assets

if ! gh release view "$tag" --repo "$repo" > /dev/null 2>&1; then
  gh release create "$tag" --repo "$repo" --prerelease \
    --title "Website files (not a Neper release)" \
    --notes "Large files of the Neper website (https://neper.info): old Neper versions."
fi

gh release upload "$tag" --repo "$repo" --clobber "$dir"/*
