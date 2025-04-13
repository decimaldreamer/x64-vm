# Use a multi-stage build to reduce the final image size
FROM ubuntu:22.04 as builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    libopengl-dev \
    libdirectx-dev \
    libvulkan-dev \
    libtensorflow-dev \
    libboost-system-dev \
    libboost-serialization-dev \
    llvm-dev \
    libpcap-dev \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy source code
COPY . /src
WORKDIR /src/build

# Build the project
RUN cmake .. && make -j$(nproc)

# Final stage
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libssl1.1 \
    libopengl0 \
    libdirectx-11 \
    libvulkan1 \
    libtensorflow2 \
    libboost-system1.74.0 \
    libboost-serialization1.74.0 \
    llvm-12 \
    libpcap0.8 \
    zlib1g \
    && rm -rf /var/lib/apt/lists/*

# Copy built binaries from builder stage
COPY --from=builder /src/build/bin /usr/local/bin
COPY --from=builder /src/build/lib /usr/local/lib

# Set environment variables
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# Create a non-root user
RUN useradd -m -s /bin/bash x64vm
USER x64vm

# Set working directory
WORKDIR /home/x64vm

# Expose ports for network virtualization
EXPOSE 80 443 8080

# Set entrypoint
ENTRYPOINT ["/usr/local/bin/x64vm"] 