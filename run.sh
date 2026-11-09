#!/usr/bin/env bash
set -euo pipefail

# Use a named PostgreSQL volume so database changes persist across containers.
# PostgreSQL 18 mounts /var/lib/postgresql, not /var/lib/postgresql/data.
docker run --rm -it --name sdmcontainer \
  -e POSTGRES_PASSWORD=sdm-local-development-only \
  -v sdm-postgres-data:/var/lib/postgresql \
  sdmcontainer:latest
