FROM randomdude/gcc-cross-i686-elf

RUN echo "deb http://archive.debian.org/debian stretch main contrib non-free" > /etc/apt/sources.list

RUN apt-get clean
RUN rm -rf /var/lib/apt/lists/*
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get update && apt-get install -y nasm
RUN apt-get update && apt-get install -y xorriso
RUN apt-get update && apt-get install -y grub-pc-bin
RUN apt-get update && apt-get install -y grub-common

VOLUME /root/env
WORKDIR /root/env