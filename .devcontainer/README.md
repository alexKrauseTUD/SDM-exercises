# SDM Docker / Dev Container

The image can build natively as **linux/arm64** (Apple Silicon) or
**linux/amd64** (Intel/AMD Linux). Docker Desktop on macOS runs Linux containers.

## IMDb dataset downloads

As of November 9, 2026, the course maintainer supplied three replacement
Nextcloud links. These links are now the Dockerfile defaults:

- `sql.zip`: `https://db4701.inf.tu-dresden.de:8443/index.php/s/DiaLzNWdprMKHBW`
- `csv.zip`: `https://db4701.inf.tu-dresden.de:8443/index.php/s/5yRYGz9G5QoHfQ3`
- `import.sql`: `https://db4701.inf.tu-dresden.de:8443/index.php/s/RBWoAiHG5t9Nnn2`

The build appends `/download` to the Nextcloud share paths, downloads the
archives and SQL importer, extracts the CSV data, and expects `sql.zip` to
contain a file named `create.sql`. If that assumption is wrong or a URL
fails, the build fails explicitly instead of building an empty teaching DB.
The source server cannot be reached from the authoring environment, so these
downloads and the archive contents still need native-host validation.

```sh
./setup.sh
./run.sh
```

With a fresh database volume, the upstream entrypoint creates the IMDb
database and imports the dataset on first startup. The server previously
required `curl --insecure`; the Dockerfile currently restricts this
workaround to the teaching-host downloads, but proper server TLS and archive
hash verification are preferred.

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
and the `imdb` database, and imports the downloaded exercise data.
Restarting uses the existing volume and does not re-import the dataset.

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
