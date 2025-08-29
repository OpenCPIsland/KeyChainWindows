#!/bin/bash
# Build script for libMemoryMonitorLinux.so and libKeyChainLinux.so
# Installs dependencies if missing, keeps terminal open, outputs build logs

# Log file
LOG_FILE="build/build.log"

# Ensure build directory exists
mkdir -p build

# Function to check command existence
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Install dependencies if missing
if ! command_exists gcc; then
    echo "gcc not found. Installing..."
    sudo apt update
    sudo apt install -y build-essential
fi

if ! command_exists make; then
    echo "make not found. Installing..."
    sudo apt update
    sudo apt install -y make
fi

# Clear previous log
> "$LOG_FILE"

# Compile function
compile_so() {
    local src_file=$1
    local output_file=$2
    echo "Building $output_file..."
    gcc -fPIC -shared -Wall -O2 "$src_file" -o "$output_file" 2>&1 | tee -a "$LOG_FILE"
    if [ ${PIPESTATUS[0]} -ne 0 ]; then
        echo "Failed to build $output_file. Check $LOG_FILE for details."
        read -p "Press enter to exit..."
        exit 1
    fi
}

# Build MemoryMonitorLinux.so
compile_so "src/MemoryMonitorLinux.c" "build/libMemoryMonitorLinux.so"

# Build KeyChainLinux.so
compile_so "src/KeyChainLinux.c" "build/libKeyChainLinux.so"

echo "Build complete! .so files are in build/"
read -p "Press enter to exit..."
