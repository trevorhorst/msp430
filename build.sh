#!/bin/bash

# # Build
# cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/msp430-toolchain.cmake -DMCU=msp430fr2433 ..
# make

# Exit immediately if a command exits with a non-zero status
set -e

# 1. Check if MCU env variable is empty, if so, set a default
if [ -z "$TARGET_MCU" ]; then
    echo "⚠️  WARNING: 'TARGET_MCU' environment variable is not set."
    export TARGET_MCU="msp430fr2433"
    echo "➡️  Defaulting to target: $TARGET_MCU"
else
    echo "✅ Target MCU detected from environment: $TARGET_MCU"
fi

# 2. Normalize string to lowercase (optional, prevents typos like 'MSP430FR2433')
TARGET_MCU=$(echo "$TARGET_MCU" | tr '[:upper:]' '[:lower:]')

# 3. Create a unique build directory name based on the MCU
BUILD_DIR="build_${TARGET_MCU}"
echo "📁 Build directory: $BUILD_DIR"

# Clean the build directory
rm -rf $BUILD_DIR

# 4. Configure and build using your CMake toolchain
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "⚙️  Configuring CMake..."
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/msp430-toolchain.cmake -DMCU="$TARGET_MCU" .. 

echo "🔨 Compiling binaries..."
make -j$(nproc)

echo "🎉 Build complete for $TARGET_MCU!"
