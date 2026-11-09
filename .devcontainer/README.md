# SDM Docker / Dev Container

The image is built natively for **linux/amd64** (x86-64) or **linux/arm64**
(Apple Silicon and other ARM64 Linux systems). Docker Desktop runs Linux
containers on macOS; no macOS-native PostgreSQL binaries are required.

## VS Code

Install Docker Desktop and the VS Code Dev Containers extension. Open the
repository and select **Dev Containers: Reopen in Container**. The configuration
builds from `.devcontainer/Dockerfile` instead of pulling a potentially
AMD64-only prebuilt image.

The PostgreSQL server starts as the container's main process. The development
container attaches as root for exercise development. PostgreSQL itself runs
as the `postgres` OS user through the official entrypoint. Use
`psql -U postgres -d imdb` inside the container.

## Terminal

From the repository root:

```sh
./setup.sh
./run.sh
```

In another terminal, run `./connect.sh`. The `run.sh` container's database
persists in the named Docker volume `sdm-postgres-data`.

## PostgreSQL initialization

PostgreSQL 18 uses `/var/lib/postgresql/18/docker` as its data directory,
under the image's `/var/lib/postgresql` volume. On an **empty database
volume**, the official entrypoint initializes the cluster, creates
`sdm_student` and `imdb`, and imports the IMDb CSVs using the supplied SQL.
Subsequent starts reuse the database without re-importing.

To start fresh, first stop the container, then (destructively) run:

```sh
docker volume rm sdm-postgres-data
```

Rebuilding the image **does not migrate an existing database volume**. Do not
reuse a PostgreSQL 17 or earlier data directory with this PostgreSQL 18
container: migrate it with a suitable PostgreSQL upgrade procedure or use a
new named volume. Do not delete a volume containing work you want to keep.

## Security and reproducibility

The password in this teaching configuration is for **local development
only**. Do not expose port 5432 to untrusted networks or deploy this image
as a production database. The dataset is fetched from the TU Dresden teaching
server at image build time. The source URLs currently require insecure TLS
verification; consider fixing the server certificate chain and pinning
archive checksums for reproducible, authenticated builds.

## Verification on each platform

```sh
docker build --platform linux/arm64 -f .devcontainer/Dockerfile -t sdm:arm64 .
docker build --platform linux/amd64 -f .devcontainer/Dockerfile -t sdm:amd64 .
```

Cross-platform builds may require emulation and take considerably longer.
For real end-to-end validation, build and start natively on an ARM64 Mac and
on an AMD64 Linux host; verify `SELECT count(*)` on an imported IMDb table
and verify restart preserves changes. The image build alone does not verify
the runtime import.
