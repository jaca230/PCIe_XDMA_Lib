#include "xdma_lib.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <device> <address> <width> [data]" << std::endl;
        return 1;
    }

    std::string device = argv[1];
    uint32_t address = std::stoul(argv[2], nullptr, 0);
    char width = argv[3][0];
    uint32_t data = 0;

    if (argc == 5) {
        data = std::stoul(argv[4], nullptr, 0);
    }

    XDMADevice dma_device(device);

    if (!dma_device.mapDevice(32 * 1024)) { // Example map size
        return 1;
    }

    if (argc == 5) {
        std::cout << "Writing data..." << std::endl;
        dma_device.write(address, data, width);
    } else {
        std::cout << "Reading data..." << std::endl;
        uint32_t read_result = dma_device.read(address, width);
        std::cout << "Read value: 0x" << std::hex << read_result << std::endl;
    }

    dma_device.unmapDevice();

    return 0;
}
