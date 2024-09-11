#!/bin/bash

# Exit on error
set -e

# Get the directory of the script
SOURCE="${BASH_SOURCE[0]}"
while [ -L "$SOURCE" ]; do
    DIR=$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )
    SOURCE=$(readlink "$SOURCE")
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
script_directory=$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )

# Define default values
OVERWRITE=false
INSTALL=false
INSTALL_PREFIX=""

# Display help message
show_help() {
    echo "Usage: ${script_directory}/build.sh [OPTIONS]"
    echo
    echo "Options:"
    echo "  --overwrite       Overwrite existing build, lib, and bin directories"
    echo "  --install         Run 'make install' to install the built files"
    echo "  --install-prefix  Specify the installation prefix (default: use CMake's default)"
    echo "  --help            Display this help message and exit"
    echo
    echo "This script configures and builds the project using CMake and Make."
    exit 0
}

# Parse command-line arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --overwrite) OVERWRITE=true ;;
        --install) INSTALL=true ;;
        --install-prefix) INSTALL_PREFIX="$2"; shift ;;
        --help) show_help ;;
        *) echo "Unknown parameter passed: $1"; show_help ;;
    esac
    shift
done

# Define build directories relative to the script directory
BUILD_DIR="${script_directory}/../build"
LIB_DIR="${script_directory}/../lib"
BIN_DIR="${script_directory}/../bin"

# Remove existing build, lib, and bin directories if overwrite is true
if [ "$OVERWRITE" = true ]; then
    echo "Overwriting existing build, lib, and bin directories..."
    rm -rf "$BUILD_DIR" || true
    rm -rf "$LIB_DIR" || true
    rm -rf "$BIN_DIR" || true
fi

# Create the build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

# Change to the build directory
cd "$BUILD_DIR"

# Run CMake to configure the project and generate Makefiles
if [ -n "$INSTALL_PREFIX" ]; then
    cmake -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" "${script_directory}/.."
else
    cmake "${script_directory}/.."
fi

# Build the project using Make
make -j

# Run make install if the --install flag is specified
if [ "$INSTALL" = true ]; then
    echo "Installing built files..."
    make install
fi

# Notify completion
echo "Build completed successfully."
if [ "$INSTALL" = true ]; then
    echo "Installation completed successfully."
fi

# Return to the original directory
cd "$script_directory"
