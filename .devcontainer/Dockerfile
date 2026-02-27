FROM ghcr.io/classpaddev/hollyhock-3:v2.2.1 AS toolchain

FROM mcr.microsoft.com/devcontainers/base:debian13 AS base

# Install required packages KEEP IN SYNC (+clangd bear nano)
RUN apt-get update -y && apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
    make libncurses6 zstd zlib1g \
    gawk wget bzip2 xz-utils unzip \
    patch libstdc++6 rsync git mold \
    nano clangd-19 bear
RUN apt-get install -y --reinstall ca-certificates
COPY --from=toolchain /toolchain /toolchain
COPY --from=toolchain /sdk /sdk
ENV PATH=$PATH:/toolchain/bin
ENV SDK_DIR=/sdk
