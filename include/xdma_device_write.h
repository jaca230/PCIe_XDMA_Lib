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

    bool writeToDevice(uint32_t address, const std::string& filename = "");
    bool writeToDevice(uint32_t address, const void* data, size_t size);
    bool writeToDevice(uint32_t address, const std::vector<std::byte>& byteArray);

    void printTransferSpeed() const;

private:
    std::vector<std::byte> buffer_;  // Use std::byte to store raw data
    std::chrono::steady_clock::time_point startTime_, endTime_;
};

#endif // XDMA_DEVICE_WRITE_H
