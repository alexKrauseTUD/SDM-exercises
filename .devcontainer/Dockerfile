# Pin the major version if you can
FROM postgres:18

EXPOSE 5432 8888
STOPSIGNAL SIGINT

ENV LANG=en_US.UTF-8
ENV LC_ALL=C.UTF-8
ARG TIMEZONE=UTC
ENV TZ=$TIMEZONE
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install dependencies (root)
RUN apt update && apt install -y \
    build-essential sudo locales tzdata procps lsof wget \
    bison flex curl pkg-config cmake llvm clang \
    libicu-dev libreadline-dev libssl-dev liblz4-dev libossp-uuid-dev libzstd-dev zlib1g-dev \
    git vim unzip zstd default-jre tmux \
    python3 python3-venv python3-pip && \
    locale-gen en_US.UTF-8 && update-locale LANG=en_US.UTF-8 && \
    rm -rf /var/lib/apt/lists/* && apt clean

# Fetch your data & SQL (root)
RUN mkdir -p /exercise/imdb \
    && curl -k -L -o /exercise/imdb/csv.zip     "https://db4701.inf.tu-dresden.de:8443/index.php/s/H7TKaEBr5JmdaNA/download/csv.zip" \
    && curl -k -L -o /exercise/imdb/create.sql  "https://db4701.inf.tu-dresden.de:8443/index.php/s/e35mDHTCZx88y6p/download/create.sql" \
    && curl -k -L -o /exercise/imdb/import.sql  "https://db4701.inf.tu-dresden.de:8443/index.php/s/bNzMwSpmQESRz6P/download/import.sql" \
    && cd /exercise/imdb && unzip csv.zip && rm csv.zip

# Use a subdir for the cluster to keep the top-level volume clean
ENV PGDATA=/var/lib/postgresql/data/pgdata
ENV PGLOGCONF="${PGDATA}/postgresql.conf"
ENV PGLOGDIR=/var/log/postgresql
# Prepare dirs and ownership (root)
RUN mkdir -p /var/lib/postgresql /var/run/postgresql "$PGDATA" \
    && chown -R postgres:postgres /var/lib/postgresql /var/run/postgresql \
    && mkdir -p "$PGLOGDIR" && chown -R postgres:postgres "$PGLOGDIR" \
    && chmod 700 "$PGDATA"

# Initialize and load DB as the postgres OS user
USER postgres
RUN initdb -D "$PGDATA" \
 && { \
      echo "logging_collector = on"; \
      echo "log_destination = 'csvlog'"; \
      echo "log_directory = '$PGLOGDIR'"; \
      echo "log_filename = 'postgresql-%Y-%m-%d_%H%M%S.log'"; \
      echo "log_rotation_age = 1d"; \
      echo "log_rotation_size = 0"; \
    } >> "$PGDATA/postgresql.conf" \
 && pg_ctl -D "$PGDATA" -o "-c listen_addresses=''" -w start \
 && psql -v ON_ERROR_STOP=1 -U postgres -d postgres -c "CREATE USER sdm_student;" \
 && psql -v ON_ERROR_STOP=1 -U postgres -d postgres -c "CREATE DATABASE imdb OWNER sdm_student TEMPLATE template0 ENCODING 'UTF8';" \
 && psql -v ON_ERROR_STOP=1 -U postgres -d imdb -f /exercise/imdb/create.sql \
 && (cd /exercise/imdb && psql -v ON_ERROR_STOP=1 -U postgres -d imdb -f import.sql) \
 && pg_ctl -D "$PGDATA" -m fast -w stop

# Final container config
USER root
WORKDIR /exercise
VOLUME /exercise

# If you want the DB to start automatically, keep the default entrypoint/cmd.
# Your previous line replaced it with a shell; choose ONE of the following:
# Option 1: run an interactive shell (you must start postgres yourself)
USER postgres
ENTRYPOINT ["/bin/bash"]

# Option 2 (recommended for a DB container): use the default entrypoint and start the server
# CMD ["postgres"]
