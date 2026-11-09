# SDM Docker / Dev Container

The image can build natively as **linux/arm64** (Apple Silicon) or
**linux/amd64** (Intel/AMD Linux). Docker Desktop on macOS runs Linux containers.

## Important: IMDb dataset currently unavailable

The original `db4701.inf.tu-dresden.de:8443` Nextcloud `csv.zip` URL
returns HTTP 404 (reported November 9, 2026). This is independent of CPU
architecture. **The default image builds and starts without IMDb tables or
records**, but it is not sufficient for exercises that query IMDb.

When valid URLs are available for the three matching teaching files (the
CSV ZIP, `create.sql`, and `import.sql`), build from the repository root:

```sh
docker build -t sdmcontainer:latest -f .devcontainer/Dockerfile \
  --build-arg IMDB_CSV_URL='https://example.edu/path/csv.zip' \
  --build-arg IMDB_CREATE_SQL_URL='https://example.edu/path/create.sql' \
  --build-arg IMDB_IMPORT_SQL_URL='https://example.edu/path/import.sql' .
```

These are **placeholders**, not replacement downloads. All three arguments
must be supplied together; failed downloads stop the build rather than
silently producing partial data. Use HTTPS URLs with trusted certificates.
Do not assume IMDb's official TSV files are compatible with the original
teaching schema and CSV importer. Ensure you have the right to use and
redistribute any dataset.

## VS Code

Install Docker Desktop and the VS Code Dev Containers extension. Open this
repository and select **Dev Containers: Reopen in Container**. VS Code builds
the Dockerfile locally, not the potentially AMD64-only prebuilt image.

PostgreSQL starts as the main process and the development session uses root.
PostgreSQL itself runs as the `postgres` OS user. In the Dev Container, use
`psql -U postgres -d imdb`.

## Terminal

```sh
./setup.sh
./run.sh
```

In another terminal, use `./connect.sh`. The local PostgreSQL data is
persisted in the `sdm-postgres-data` named volume.

## Initialization and migration

PostgreSQL 18's data directory is `/var/lib/postgresql/18/docker`, inside the
`/var/lib/postgresql` volume. At first start on an **empty volume**, the
official entrypoint initializes PostgreSQL, creates the `sdm_student` role
and the `imdb` database, and imports exercise data **if supplied**. Without
data, it prints a warning and creates an empty `imdb` database. Restarting
uses the existing volume, so later supplying a dataset does not automatically
trigger an import.

Only after ensuring there is no data to preserve, stop the container and
reset the standalone named volume with:

```sh
docker volume rm sdm-postgres-data
```

Do not reuse a PostgreSQL 17 volume with the PostgreSQL 18 image; perform a
supported major-version migration or use a separate new volume.

## Validation

```sh
docker build --platform linux/arm64 -f .devcontainer/Dockerfile -t sdm:arm64 .
docker build --platform linux/amd64 -f .devcontainer/Dockerfile -t sdm:amd64 .
```

Cross-platform builds may need emulation. End-to-end validation requires
native Apple Silicon and AMD64 runs, confirmation that PostgreSQL starts,
and — once the dataset is available — verification of imported table counts
and persistence after restart.

The password in the local teaching configuration is not safe for production.
Do not publish PostgreSQL to untrusted networks.
