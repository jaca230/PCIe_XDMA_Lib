#ifndef XDMA_DEVICE_H
#define XDMA_DEVICE_H

#include "xdma_device_read.h"
#include "xdma_device_write.h"
#include "xdma_device_control.h"
#include <vector>
#include <string>

class XDMADevice {
public:
    // Constructor takes the device prefix
    XDMADevice(const std::string& devicePrefix);

    // Destructor
    ~XDMADevice();

    // Initialize all devices
    bool initialize();

    // Methods for reading and writing to the device
    std::vector<char> readFromDevice(uint32_t address, size_t size, int channelIndex);
    bool writeToDevice(uint32_t address, size_t size, const char* data, int channelIndex);
    bool writeToDevice(uint32_t address, size_t size, const std::string& filename, int channelIndex);

    // Methods for printing transfer speeds and hex dumps
    void printReadTransferSpeed(int channelIndex) const;
    void printWriteTransferSpeed(int channelIndex) const;
    void printReadHexDump(size_t size, int channelIndex) const;

    // Methods for control device
    int getH2CChannelCount();
    int getC2HChannelCount();
    bool isStreaming() const;

private:
    std::string devicePrefix_;
    XDMADeviceControl controlDevice_;
    std::vector<XDMADeviceRead> readDevices_;
    std::vector<XDMADeviceWrite> writeDevices_;

    // Constants
    static const std::string CONTROL_DEVICE_SUFFIX;
    static const std::string READ_DEVICE_SUFFIX;
    static const std::string WRITE_DEVICE_SUFFIX;
    static const int INVALID_CHANNEL_INDEX;
};

#endif // XDMA_DEVICE_H
