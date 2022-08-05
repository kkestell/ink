#!/usr/bin/env bash

podman build --tag=ink .
podman run \
    -it \
    --rm \
    --mount type=bind,source="$(pwd)",target=/ink \
    ink \
    /bin/bash
