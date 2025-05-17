FROM docker.io/ubuntu:20.04

ARG UID=1000
ARG GID=1000

# Create group and user
RUN groupadd -g $GID usergroup && \
    useradd -m -u $UID -g $GID -s /bin/bash username

# Install necessary packages and sudo
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
  make \
  clang \
  clang-format \
  clang-tidy \
  lld \
  util-linux \
  dosfstools \
  sudo \
  && rm -rf /var/lib/apt/lists/*

# Grant sudo privileges to the user with no password required
RUN echo "username ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Create loop devices
RUN for i in $(seq 0 7); do mknod /dev/loop$i b 7 $i && chmod 0660 /dev/loop$i; done

# Switch to the user
USER $UID:$GID

WORKDIR /ink
