#!/usr/bin/env bash

docker build --tag=ink .
docker run \
    -it \
    --rm \
    --mount type=bind,source="$(pwd)",target=/ink \
    ink \
    /bin/bash
