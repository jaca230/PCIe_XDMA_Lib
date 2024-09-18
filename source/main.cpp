#include "xdma_device.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib> // for std::exit
#include <sstream> // for std::ostringstream
#include <iomanip> // For std::setw and std::left
#include <string>
#include <cstddef>  // for std::byte

// Helper function to format lines to a fixed width
std::string formatLine(const std::string& text, size_t value) {
    std::ostringstream oss;
    oss << std::left << std::setw(16) << std::setfill(' ') << (text + std::to_string(value));
    return oss.str();
}

void printByteBuffer(const std::vector<std::byte>& buffer) {
    std::cout << "Byte buffer contents:\n";
    for (std::size_t i = 0; i < buffer.size(); ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(buffer[i]) << ' ';
        if ((i + 1) % 16 == 0) { // Print 16 bytes per line
            std::cout << '\n';
        }
    }
    std::cout << '\n';
}

int main(int argc, char* argv[]) {
    if (argc < 5 || argc > 6) { // Correct argument count check
        std::cerr << "Usage: " << argv[0] << " <device_prefix> <address> <size> <channel_index> [printout]\n";
        return EXIT_FAILURE;
    }

    std::string devicePrefix = argv[1];
    uint32_t address = std::stoul(argv[2], nullptr, 0); // Convert address
    size_t size = std::stoul(argv[3], nullptr, 0); // Convert size
    int channelIndex = std::stoi(argv[4]); // Convert channel index

    // Determine printout flag
    bool printout = (argc == 6) ? (std::string(argv[5]) == "true") : (size <= 1024);

    // Create the XDMADevice object
    XDMADevice xdmaDevice(devicePrefix);

    // Initialize the device
    if (!xdmaDevice.initialize()) {
        std::cerr << "Failed to initialize the device.\n";
        return EXIT_FAILURE;
    }

    // Generate test data
    std::ostringstream oss;
    oss << formatLine("Transfer test:", 0); // First line
    oss << formatLine("Size:", size); // Second line
    oss << formatLine("Channel:", channelIndex); // Third line

    std::string pattern = "16B Example Line";
    std::string filler;
    size_t num_repeats = (size - oss.str().size()) / pattern.size();
    for (size_t i = 0; i < num_repeats; ++i) {
        filler.append(pattern);
    }
    std::string message = oss.str() + filler;

    // Ensure the message size matches the requested size
    message = message.substr(0, size);

    // Write to the device
    auto start = std::chrono::high_resolution_clock::now();
    if (xdmaDevice.writeToDevice(address, message.data(), size, channelIndex)) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Write operation successful. Time taken: " << elapsed.count() << " seconds.\n";
        xdmaDevice.printWriteTransferSpeed(channelIndex);
    } else {
        std::cerr << "Failed to write to the device.\n";
        return EXIT_FAILURE;
    }

    // Read from the device
    start = std::chrono::high_resolution_clock::now();
    std::vector<std::byte> byteBuffer = xdmaDevice.readFromDevice(address, size, channelIndex);
    if (!byteBuffer.empty()) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Read operation successful, " << byteBuffer.size() << " bytes read. Time taken: " << elapsed.count() << " seconds.\n";

        if (printout) {
            xdmaDevice.printReadHexDump(size, channelIndex); // Print hex dump of the read data
        }
        xdmaDevice.printReadTransferSpeed(channelIndex); // Print read transfer speed
    } else {
        std::cerr << "Failed to read from the device.\n";
        return EXIT_FAILURE;
    }

    // Get the number of h2c channels and c2h channels
    int h2cChannels = xdmaDevice.getH2CChannelCount();
    int c2hChannels = xdmaDevice.getC2HChannelCount();
    std::cout << "Number of h2c channels: " << h2cChannels << "\n";
    std::cout << "Number of c2h channels: " << c2hChannels << "\n";

    // Determine if the core is memory-mapped or streaming
    bool isStreaming = xdmaDevice.isStreaming();
    std::cout << "The PCIe DMA core is " << (isStreaming ? "streaming." : "memory-mapped.") << "\n";

    return EXIT_SUCCESS;
}
