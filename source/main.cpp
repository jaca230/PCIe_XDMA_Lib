#include "xdma_device.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <device> <address> <size>\n";
        return EXIT_FAILURE;
    }

    std::string device = argv[1];
    uint32_t address = std::stoul(argv[2], nullptr, 0);
    size_t size = std::stoul(argv[3], nullptr, 0);

    // Create the XDMADevice object
    XDMADevice xdmaDevice(device);

    // Initialize the device (open the device)
    if (!xdmaDevice.initialize()) {
        return EXIT_FAILURE;
    }

    // Read from the device
    ssize_t bytes = xdmaDevice.readFromDevice(address, size);
    if (bytes > 0) {
        xdmaDevice.printTransferSpeed();
        xdmaDevice.printHexDump(size);
    }

    return EXIT_SUCCESS;
}
