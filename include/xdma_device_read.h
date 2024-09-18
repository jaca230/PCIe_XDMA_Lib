#ifndef XDMA_DEVICE_READ_H
#define XDMA_DEVICE_READ_H

#include "xdma_device_base.h"
#include <vector>
#include <chrono>

class XDMADeviceRead : public XDMADeviceBase {
public:
    XDMADeviceRead(const std::string& device);
    ~XDMADeviceRead();

    std::vector<std::byte> readFromDevice(uint32_t address, size_t size);
    void printTransferSpeed() const;
    void printHexDump(size_t size, size_t word_size = 1) const;

private:
    std::vector<std::byte> buffer_;  // Use std::byte to store raw data
    std::chrono::steady_clock::time_point startTime_, endTime_;
};

#endif // XDMA_DEVICE_READ_H
