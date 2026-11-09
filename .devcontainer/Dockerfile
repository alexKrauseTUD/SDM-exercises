# Native linux/amd64 and linux/arm64 (including Docker Desktop on Apple Silicon).
FROM postgres:18-bookworm

EXPOSE 5432 8888
ENV LANG=en_US.UTF-8 \
    LC_ALL=C.UTF-8
ARG TIMEZONE=UTC
ENV TZ=${TIMEZONE}

RUN set -eux; \
    ln -snf "/usr/share/zoneinfo/${TZ}" /etc/localtime; \
    echo "${TZ}" > /etc/timezone; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
        build-essential sudo locales tzdata procps lsof wget \
        bison flex curl pkg-config cmake llvm clang \
        libicu-dev libreadline-dev libssl-dev liblz4-dev libossp-uuid-dev \
        libzstd-dev zlib1g-dev git vim unzip zstd default-jre tmux \
        python3 python3-venv python3-pip; \
    locale-gen en_US.UTF-8; \
    update-locale LANG=en_US.UTF-8; \
    rm -rf /var/lib/apt/lists/*

# Nextcloud share URLs supplied by the course maintainer on 2026-11-09.
# Unlike the original link, the schema is distributed as sql.zip, not create.sql.
# These public URLs may be overridden with build args.
ARG IMDB_SQL_ZIP_URL="https://db4701.inf.tu-dresden.de:8443/index.php/s/DiaLzNWdprMKHBW/download"
ARG IMDB_CSV_URL="https://db4701.inf.tu-dresden.de:8443/index.php/s/5yRYGz9G5QoHfQ3/download"
ARG IMDB_IMPORT_SQL_URL="https://db4701.inf.tu-dresden.de:8443/index.php/s/RBWoAiHG5t9Nnn2/download"

# The teaching Nextcloud server previously required --insecure.
# The option is limited to these three course-host downloads.
RUN set -eu; \
    mkdir -p /exercise/imdb /tmp/imdb-sql; \
    curl --fail --insecure --show-error --silent --location --retry 3 \
        -o /tmp/sql.zip "$IMDB_SQL_ZIP_URL"; \
    curl --fail --insecure --show-error --silent --location --retry 3 \
        -o /exercise/imdb/csv.zip "$IMDB_CSV_URL"; \
    curl --fail --insecure --show-error --silent --location --retry 3 \
        -o /exercise/imdb/import.sql "$IMDB_IMPORT_SQL_URL"; \
    unzip -tq /tmp/sql.zip; \
    unzip -q /tmp/sql.zip -d /tmp/imdb-sql; \
    schema="$(find /tmp/imdb-sql -type f -name create.sql -print -quit)"; \
    if [ -z "$schema" ]; then \
        echo >&2 "ERROR: sql.zip does not contain create.sql. Check archive contents and update the initialization workflow."; \
        find /tmp/imdb-sql -type f -print >&2; \
        exit 1; \
    fi; \
    cp "$schema" /exercise/imdb/create.sql; \
    cd /exercise/imdb; \
    unzip -tq csv.zip; \
    unzip -q csv.zip; \
    rm csv.zip; \
    rm -rf /tmp/imdb-sql /tmp/sql.zip; \
    test -s /exercise/imdb/create.sql; \
    test -s /exercise/imdb/import.sql

# PostgreSQL 18's upstream entrypoint initializes /var/lib/postgresql/18/docker
# in its declared /var/lib/postgresql volume and executes this hook once.
COPY .devcontainer/init-imdb.sh /docker-entrypoint-initdb.d/10-imdb.sh
RUN chmod 0755 /docker-entrypoint-initdb.d/10-imdb.sh

WORKDIR /exercise
# Keep docker-entrypoint.sh and the upstream CMD (postgres).
