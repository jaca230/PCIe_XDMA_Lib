cmake_minimum_required(VERSION 3.10)

# Project name
project(PCIe_DMA_Readout)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the header files
include_directories(include)

# Add source files
set(SOURCE_FILES
    source/main.cpp
    source/xdma_device.cpp
)

# Create the static library
add_library(xdma_device STATIC source/xdma_device.cpp)

# Set the output directory for the library and executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Create the executable from main.cpp
add_executable(PCIe_DMA_Readout source/main.cpp)

# Link the library to the executable
target_link_libraries(PCIe_DMA_Readout xdma_device)

# Install targets
install(TARGETS PCIe_DMA_Readout
    RUNTIME DESTINATION bin
)

install(TARGETS xdma_device
    ARCHIVE DESTINATION lib
)
