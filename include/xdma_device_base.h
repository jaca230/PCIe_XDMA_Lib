#ifndef XDMA_DEVICE_BASE_H
#define XDMA_DEVICE_BASE_H

#include <string>

class XDMADeviceBase {
public:
    XDMADeviceBase(const std::string& device);
    virtual ~XDMADeviceBase();

    virtual bool initialize();
    virtual void openDevice();
    virtual void closeDevice();
    bool isDeviceOpen() const;

protected:
    std::string device_;
    int fd_;

private:
    void checkDeviceOpen() const;
};

#endif // XDMA_DEVICE_BASE_H
