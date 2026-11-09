#!/usr/bin/env bash
set -euo pipefail

# Native build: amd64 on Intel/Linux, arm64 on Apple Silicon.
# Run from any directory; the Docker build context must be the repository root.
repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
docker build -t sdmcontainer:latest -f "$repo_dir/.devcontainer/Dockerfile" "$repo_dir"
