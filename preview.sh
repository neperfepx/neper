#!/bin/bash
# Download the latest preview of the website (built from website-devel in neperfepx/neper-dev)
# and serve it locally.
#
# Usage: ./preview.sh [<port>]   (default port: 8000)
#
# Requires the GitHub CLI (gh), logged in with access to neperfepx/neper-dev.

set -euo pipefail

repo=neperfepx/neper-dev
branch=website-devel
port=${1:-8000}

run=$(gh api "repos/$repo/actions/workflows/website.yaml/runs?branch=$branch&status=success&per_page=1" \
        --jq '.workflow_runs[0].id // empty')
if [ -z "$run" ]; then
  echo "No successful run of the website workflow on $branch in $repo." >&2
  exit 1
fi

info=$(gh api "repos/$repo/actions/runs/$run" --jq '"\(.head_sha[0:9]) \(.updated_at)"')
dir=${TMPDIR:-/tmp}/neper-website-preview-$run

if [ ! -f "$dir/index.html" ]; then
  rm -rf "$dir"
  echo "Downloading the preview of run $run (about 90 MB, this can take a minute or two)..."
  if ! gh run download "$run" --repo "$repo" --name website-preview --dir "$dir"; then
    echo "Could not download the preview of run $run (artifacts expire after 7 days):" >&2
    echo "rerun the workflow with: gh workflow run website.yaml --repo $repo --ref $branch" >&2
    exit 1
  fi
fi

echo "Preview of $branch at $info (run $run), in $dir"
echo "Open http://localhost:$port (Ctrl+C to stop)"
python3 -m http.server --directory "$dir" --bind 127.0.0.1 "$port"
