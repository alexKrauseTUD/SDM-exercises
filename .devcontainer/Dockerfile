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

# The original TU Dresden Nextcloud links now return 404.
# A working dataset is OPTIONAL at build time, but is required for IMDb exercises.
# Provide all three URLs together using --build-arg when new URLs are available.
ARG IMDB_CSV_URL=""
ARG IMDB_CREATE_SQL_URL=""
ARG IMDB_IMPORT_SQL_URL=""
RUN set -eu; \
    mkdir -p /exercise/imdb; \
    if [ -z "$IMDB_CSV_URL" ] && [ -z "$IMDB_CREATE_SQL_URL" ] && [ -z "$IMDB_IMPORT_SQL_URL" ]; then \
        echo >&2 "NOTICE: IMDb dataset not configured; building without IMDb tables/data."; \
    elif [ -n "$IMDB_CSV_URL" ] && [ -n "$IMDB_CREATE_SQL_URL" ] && [ -n "$IMDB_IMPORT_SQL_URL" ]; then \
        curl --fail --show-error --silent --location --retry 3 -o /exercise/imdb/csv.zip "$IMDB_CSV_URL"; \
        curl --fail --show-error --silent --location --retry 3 -o /exercise/imdb/create.sql "$IMDB_CREATE_SQL_URL"; \
        curl --fail --show-error --silent --location --retry 3 -o /exercise/imdb/import.sql "$IMDB_IMPORT_SQL_URL"; \
        cd /exercise/imdb; \
        unzip -tq csv.zip; \
        unzip -q csv.zip; \
        rm csv.zip; \
    else \
        echo >&2 "ERROR: Set all three IMDb build args or none of them."; \
        exit 1; \
    fi

# PostgreSQL 18's upstream entrypoint initializes /var/lib/postgresql/18/docker
# in its declared /var/lib/postgresql volume and executes this hook once.
COPY .devcontainer/init-imdb.sh /docker-entrypoint-initdb.d/10-imdb.sh
RUN chmod 0755 /docker-entrypoint-initdb.d/10-imdb.sh

WORKDIR /exercise
# Keep docker-entrypoint.sh and the upstream CMD (postgres).
