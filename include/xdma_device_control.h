#ifndef XDMA_DEVICE_CONTROL_H
#define XDMA_DEVICE_CONTROL_H

#include "xdma_device_base.h"

class XDMADeviceControl : public XDMADeviceBase {
public:
    XDMADeviceControl(const std::string& device);
    ~XDMADeviceControl();

    // Additional methods specific to control device
};

#endif // XDMA_DEVICE_CONTROL_H
