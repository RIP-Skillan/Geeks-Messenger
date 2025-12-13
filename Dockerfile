# Build Stage
FROM ubuntu:22.04 AS builder

# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make \
    libboost-all-dev \
    libprotobuf-dev \
    protobuf-compiler \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy source code
COPY . .

# Build
RUN rm -rf build && mkdir build && cd build && \
    cmake .. && \
    cmake --build .

# Runtime Stage
FROM ubuntu:22.04

# Install runtime dependencies
# Note: These versions match Ubuntu 22.04 defaults
RUN apt-get update && apt-get install -y \
    libboost-system1.74.0 \
    libprotobuf23 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy binaries from builder
COPY --from=builder /app/build/src/server/server_app .
COPY --from=builder /app/build/src/client/client_app .

# Expose server port
EXPOSE 12345

# Default command runs the server
CMD ["./server_app", "12345"]
