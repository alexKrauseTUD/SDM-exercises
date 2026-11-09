# The official PostgreSQL image supports native linux/amd64 and linux/arm64.
# Pin the Debian release to keep the development toolchain predictable.
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

# Download architecture-independent teaching data.
# The teaching server currently needs --insecure (as in the original image);
# avoid reusing this setting for other downloads.
RUN set -eux; \
    mkdir -p /exercise/imdb; \
    curl --fail --insecure --location --retry 3 \
        -o /exercise/imdb/csv.zip \
        "https://db4701.inf.tu-dresden.de:8443/index.php/s/H7TKaEBr5JmdaNA/download/csv.zip"; \
    curl --fail --insecure --location --retry 3 \
        -o /exercise/imdb/create.sql \
        "https://db4701.inf.tu-dresden.de:8443/index.php/s/e35mDHTCZx88y6p/download/create.sql"; \
    curl --fail --insecure --location --retry 3 \
        -o /exercise/imdb/import.sql \
        "https://db4701.inf.tu-dresden.de:8443/index.php/s/bNzMwSpmQESRz6P/download/import.sql"; \
    cd /exercise/imdb; \
    unzip -q csv.zip; \
    rm csv.zip

# Let PostgreSQL's official entrypoint initialize its PG18 data directory
# (/var/lib/postgresql/18/docker) in a persistent /var/lib/postgresql volume.
# Init scripts execute once, only when the database volume is empty.
COPY .devcontainer/init-imdb.sh /docker-entrypoint-initdb.d/10-imdb.sh
RUN chmod 0755 /docker-entrypoint-initdb.d/10-imdb.sh

WORKDIR /exercise
# Preserve the upstream ENTRYPOINT and CMD (docker-entrypoint.sh postgres).
