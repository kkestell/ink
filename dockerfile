FROM nimlang/nim
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
  make \
  clang \
  lld \
  && rm -rf /var/lib/apt/lists/*
WORKDIR /ink
