#ifndef XDMA_DEVICE_H
#define XDMA_DEVICE_H

#include "xdma_device_read.h"
#include "xdma_device_write.h"
#include "xdma_device_control.h"

class XDMADevice {
public:
    XDMADevice(const std::string& readDevice, const std::string& writeDevice, const std::string& controlDevice);
    ~XDMADevice();

    bool initialize();  // Initialize all devices
    std::vector<char> readFromDevice(uint32_t address, size_t size);  // Read from the read device
    bool writeToDevice(uint32_t address, size_t size, const char* data);  // Write to the write device
    bool writeToDevice(uint32_t address, size_t size, const std::string& filename);  // Write to the write device
    void printReadTransferSpeed() const;  // Print transfer speed for reading
    void printWriteTransferSpeed() const;  // Print transfer speed for writing
    void printReadHexDump(size_t size) const;  // Print hex dump for reading
    // Add additional control methods if needed

private:
    XDMADeviceRead readDevice_;
    XDMADeviceWrite writeDevice_;
    XDMADeviceControl controlDevice_;
};

#endif // XDMA_DEVICE_H
