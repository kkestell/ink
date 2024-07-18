FROM docker.io/ubuntu:20.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
  just \
  make \
  clang \
  clang-format \
  clang-tidy \
  lld \
  parted \
  mtools \
  udev \
  && rm -rf /var/lib/apt/lists/*
WORKDIR /ink
