#!/bin/sh
set -eu

# Called only when the official entrypoint initializes a NEW database volume.
psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" \
    -c 'CREATE USER sdm_student;'
psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" \
    -c "CREATE DATABASE imdb OWNER sdm_student TEMPLATE template0 ENCODING 'UTF8';"

if [ ! -s /exercise/imdb/create.sql ] || [ ! -s /exercise/imdb/import.sql ]; then
    echo >&2 "WARNING: IMDb schema/data not provided. The imdb database exists but has no exercise tables."
    echo >&2 "Restore a complete dataset and initialize a new volume to import it."
    exit 0
fi

psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname imdb \
    -f /exercise/imdb/create.sql
cd /exercise/imdb
psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname imdb \
    -f import.sql
