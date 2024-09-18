
# PCIe_XDMA_Lib
(Written by ChatGPT, reviewed by developer)

## Overview

PCIe_XDMA_Lib is a C++ library for interacting with PCIe DMA devices. This repository includes both a static library and an executable for reading from and writing to PCIe DMA devices.

## Project Structure

- `source/`: Contains the source code for the library and executable.
- `include/`: Header files for the library.
- `lib/`: Directory where the static library will be installed.
- `bin/`: Directory where the executable will be installed.
- `build/`: Directory where the build files and temporary files are placed.

## Building and Installing

### Requirements

- CMake (version 3.10 or higher)
- A C++ compiler (e.g., g++, clang++)

### Build Instructions

1. **Clone the Repository**

   ```bash
   git clone https://github.com/jaca230/PCIe_XDMA_Lib.git
   cd PCIe_XDMA_Lib
   ```

2. **Run the Build Script**

   By default, the build script will create the necessary directories, build the project, and place the output files in the `build`, `lib`, and `bin` directories.

   ```bash
   ./build.sh
   ```

   **Options:**
   - `--overwrite` : Overwrite existing `build`, `lib`, and `bin` directories.
   - `--install`   : Run `make install` after building. You can specify an install prefix by setting the `CMAKE_INSTALL_PREFIX` environment variable, e.g., `CMAKE_INSTALL_PREFIX=/your/custom/path`.

   Example with overwrite and custom install path:

   ```bash
   ./build.sh --overwrite --install
   ```

3. **Building without the Script**

   If you prefer to use CMake and Make manually:

   ```bash
   mkdir -p build
   cd build
   cmake ..
   make
   ```

   To install the files:

   ```bash
   make install
   ```

   By default, files will be installed to `/usr/local/bin` for executable, `/usr/local/lib` for libraries. and `/usr/local/include/` for headers. You can change this by setting the `CMAKE_INSTALL_PREFIX` variable:

   ```bash
   cmake -DCMAKE_INSTALL_PREFIX=/your/custom/path ..
   make
   make install
   ```

## Usage

After building and installing, you can link against the `libxdma_device.a` library and use the `PCIe_DMA_Readout` executable as needed.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
