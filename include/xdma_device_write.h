#ifndef XDMA_DEVICE_WRITE_H
#define XDMA_DEVICE_WRITE_H

#include "xdma_device_base.h"
#include <vector>
#include <string>
#include <chrono>

class XDMADeviceWrite : public XDMADeviceBase {
public:
    XDMADeviceWrite(const std::string& device);
    ~XDMADeviceWrite();

    bool writeToDevice(uint32_t address, size_t size, const std::string& filename = "");
    bool writeToDevice(uint32_t address, size_t size, const char* data);

    void printTransferSpeed() const;

private:
    std::vector<char> buffer_;
    std::chrono::steady_clock::time_point startTime_, endTime_;
};

#endif // XDMA_DEVICE_WRITE_H
