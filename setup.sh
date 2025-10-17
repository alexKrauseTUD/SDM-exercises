#!/bin/bash

docker build -t sdmcontainer --build-arg TIMEZONE=$(cat /etc/timezone) .