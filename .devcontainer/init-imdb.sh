#!/bin/sh
set -eu

# Executed by the official postgres entrypoint on FIRST initialization only.
# The entrypoint's temporary server is already running and uses the local socket.
psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" \
    -c 'CREATE USER sdm_student;'
psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" \
    -c "CREATE DATABASE imdb OWNER sdm_student TEMPLATE template0 ENCODING 'UTF8';"

psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname imdb \
    -f /exercise/imdb/create.sql
cd /exercise/imdb
psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname imdb \
    -f import.sql
