FROM quay.io/pjbaioni/amsc_mk:2025

USER root

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends gdb \
    && rm -rf /var/lib/apt/lists/*

USER ubuntu