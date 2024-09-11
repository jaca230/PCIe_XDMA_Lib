#ifndef XDMA_DEVICE_H
#define XDMA_DEVICE_H

#include <string>
#include <vector>
#include <chrono>

class XDMADevice {
public:
    XDMADevice(const std::string& device);
    ~XDMADevice();

    bool initialize();  // Initialize the device (open it)
    ssize_t readFromDevice(uint32_t address, size_t size);  // Read from the device at a specific address
    void printTransferSpeed() const;  // Print transfer speed for the last read
    void printHexDump(size_t size) const;  // Print the hex dump of the read buffer (up to `size` bytes)

private:
    std::string device_;
    int fd_;
    std::vector<char> buffer_;

    std::chrono::steady_clock::time_point startTime_, endTime_;

    void openDevice();  // Open the device
    void closeDevice();  // Close the device
};

#endif // XDMA_DEVICE_H
