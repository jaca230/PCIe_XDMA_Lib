#ifndef XDMA_DEVICE_READ_H
#define XDMA_DEVICE_READ_H

#include "xdma_device_base.h"
#include <vector>
#include <chrono>

class XDMADeviceRead : public XDMADeviceBase {
public:
    XDMADeviceRead(const std::string& device);
    ~XDMADeviceRead();

    ssize_t readFromDevice(uint32_t address, size_t size);
    void printTransferSpeed() const;
    void printHexDump(size_t size) const;

private:
    std::vector<char> buffer_;
    std::chrono::steady_clock::time_point startTime_, endTime_;
};

#endif // XDMA_DEVICE_READ_H
