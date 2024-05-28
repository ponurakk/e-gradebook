# Use Debian as base image
FROM debian:latest

# Install necessary packages
RUN apt-get update && apt-get install -y \
      cmake \
      ninja-build \
      g++ \
      libsqlite3-dev \
      wget \
      libboost-all-dev \
      && rm -rf /var/lib/apt/lists/*

# Install Crow library
WORKDIR /tmp
RUN wget "https://github.com/CrowCpp/Crow/releases/download/v1.0%2B5/crow-v1.0+5.deb" && \
    dpkg -i "crow-v1.0+5.deb" && \
    rm "crow-v1.0+5.deb"

# Set the working directory
WORKDIR /app

# Copy the source code into the container
COPY . .

# Configure and build the project
CMD mkdir -p build && cd build && cmake -GNinja .. && ninja && cd .. && ./build/e-gradebook
