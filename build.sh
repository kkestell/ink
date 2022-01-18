#!/usr/bin/env bash

pushd src
pushd boot
make clean && make
popd
pushd kernel
make clean && make
popd