#!/usr/bin/env bash
set -euo pipefail

# Connect to the running PostgreSQL server inside the container.
docker exec -it sdmcontainer psql -U postgres -d imdb
