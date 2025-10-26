#!/bin/bash

#docker build -t sdmcontainer --build-arg TIMEZONE=$(cat /etc/timezone) ./.devcontainer

docker pull jpietrzyktud/sdm:latest
docker tag jpietrzyktud/sdm:latest sdmcontainer:latest