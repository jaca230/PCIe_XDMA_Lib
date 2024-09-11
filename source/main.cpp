#include "xdma_device.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib> // for std::exit

int main(int argc, char* argv[]) {
    if (argc != 6) { // Correct argument count check
        std::cerr << "Usage: " << argv[0] << " <read_device> <write_device> <control_device> <address> <size>\n";
        return EXIT_FAILURE;
    }

    std::string readDevicePath = argv[1];
    std::string writeDevicePath = argv[2];
    std::string controlDevicePath = argv[3];
    uint32_t address = std::stoul(argv[4], nullptr, 0); // Convert address
    size_t size = std::stoul(argv[5], nullptr, 0); // Convert size

    // Create the XDMADevice object
    XDMADevice xdmaDevice(readDevicePath, writeDevicePath, controlDevicePath);

    // Initialize the device
    if (!xdmaDevice.initialize()) {
        std::cerr << "Failed to initialize the device.\n";
        return EXIT_FAILURE;
    }

    // Generate test data
    std::vector<char> testData(size, 0xAC);  // Example data, fill with 0xAB

    // Write to the device
    auto start = std::chrono::high_resolution_clock::now();
    if (xdmaDevice.writeToDevice(address, size, testData.data())) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Write operation successful. Time taken: " << elapsed.count() << " seconds.\n";
        xdmaDevice.printWriteTransferSpeed();
    } else {
        std::cerr << "Failed to write to the device.\n";
        return EXIT_FAILURE;
    }

    // Read from the device
    start = std::chrono::high_resolution_clock::now();
    ssize_t bytesRead = xdmaDevice.readFromDevice(address, size);
    if (bytesRead >= 0) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Read operation successful, " << bytesRead << " bytes read. Time taken: " << elapsed.count() << " seconds.\n";
        xdmaDevice.printReadHexDump(size);
        xdmaDevice.printReadTransferSpeed();
    } else {
        std::cerr << "Failed to read from the device.\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
