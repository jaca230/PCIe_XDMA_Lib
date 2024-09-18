#include "xdma_device.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib> // for std::exit
#include <sstream> // for std::ostringstream>
#include <iomanip> // For std::setw and std::left
#include <string>

// Helper function to format lines to a fixed width
std::string formatLine(const std::string& text, size_t value) {
    std::ostringstream oss;
    oss << text << value;
    std::string line = oss.str();
    if (line.size() < 16) {
        line += std::string(16 - line.size(), ' '); // Add spaces if the line is shorter than 16 characters
    } else if (line.size() > 16) {
        line = line.substr(0, 16); // Trim if the line is longer than 16 characters
    }
    return line;
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
    oss << " Transfer  test "; // First line
    oss << "----------------"; // Second line

    // Format the lines to be exactly 16 characters wide
    oss << formatLine("Size: ", size); // Third line
    oss << formatLine("Channel: ", channelIndex); // Fourth line

    // Fill the remaining space with "*16ByteTestLine*"
    std::string pattern = "16B Example Line";
    std::string filler;
    size_t num_repeats = (size - oss.str().size()) / pattern.size();
    for (size_t i = 0; i < num_repeats; ++i) {
        filler.append(pattern);
    }
    std::string message = oss.str() + filler;

    // Ensure the message size matches the requested size
    message = message.substr(0, size);

    // Convert the message to a vector of chars
    std::vector<char> testData(message.begin(), message.end());

    // Write to the device
    auto start = std::chrono::high_resolution_clock::now();
    if (xdmaDevice.writeToDevice(address, size, testData.data(), channelIndex)) {
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
    std::vector<char> buffer = xdmaDevice.readFromDevice(address, size, channelIndex);
    if (!buffer.empty()) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Read operation successful, " << buffer.size() << " bytes read. Time taken: " << elapsed.count() << " seconds.\n";
        if (printout) {
            xdmaDevice.printReadHexDump(buffer.size(), channelIndex); // Update this method to use the buffer
        }
        xdmaDevice.printReadTransferSpeed(channelIndex); // Update this method if needed
    } else {
        std::cerr << "Failed to read from the device.\n";
        return EXIT_FAILURE;
    }

    // Get the number of h2c channels and c2h channels
    int h2cChannels = xdmaDevice.getH2CChannelCount();
    int c2hChannels = xdmaDevice.getC2HChannelCount();
    std::cout << "Number of enabled h2c channels: " << h2cChannels << "\n";
    std::cout << "Number of enabled c2h channels: " << c2hChannels << "\n";

    // Determine if the core is memory-mapped or streaming
    bool isStreaming = xdmaDevice.isStreaming();
    std::cout << "The PCIe DMA core is " << (isStreaming ? "streaming." : "memory-mapped.") << "\n";

    return EXIT_SUCCESS;
}
