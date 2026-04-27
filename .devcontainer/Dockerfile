# Use the latest Ubuntu as the base image
FROM ubuntu:latest

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required dependencies
RUN apt-get update && \
    apt-get install -y wget && \
    apt-get install -y git \ 
    build-essential \
    xsltproc \
    cmake libssl-dev \
    libwebsockets-dev \
    docbook-xsl \
    vim \
    openssl \
    net-tools \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Create the 'dhawal' user
RUN groupadd dhawal \
    && useradd -g dhawal -m -s /bin/bash dhawal \
    && apt-get update && apt-get install -y sudo \
    && echo "dhawal ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Set the active user
USER dhawal
ENV HOME=/home/dhawal