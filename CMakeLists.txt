cmake_minimum_required(VERSION 3.10)

# Project name
project(PCIe_DMA_Readout)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the header files
include_directories(include)

# Automatically gather all source files in the source directory
file(GLOB_RECURSE SOURCE_FILES "source/*.cpp")

# Create the static library
add_library(xdma_device STATIC ${SOURCE_FILES})

# Create the executable from the main.cpp file
add_executable(PCIe_DMA_Readout source/main.cpp)

# Set the output directory for the library and executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Explicitly set the output directory for the library
set_target_properties(xdma_device PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)

# Explicitly set the output directory for the executable
set_target_properties(PCIe_DMA_Readout PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# Link the library to the executable
target_link_libraries(PCIe_DMA_Readout xdma_device)

# Find and link pthread
find_package(Threads REQUIRED)
target_link_libraries(PCIe_DMA_Readout ${CMAKE_THREAD_LIBS_INIT})

# Custom command to create output directories if they do not exist
add_custom_target(create_output_dirs
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Creating output directories"
)

# Ensure the custom command runs before building
add_dependencies(PCIe_DMA_Readout create_output_dirs)
add_dependencies(xdma_device create_output_dirs)

# Install targets
install(TARGETS PCIe_DMA_Readout
    RUNTIME DESTINATION bin
)

install(TARGETS xdma_device
    ARCHIVE DESTINATION lib
)

# Install header files
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
