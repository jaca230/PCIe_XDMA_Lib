#include "xdma_device_base.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <iostream>
#include <stdexcept>


XDMADeviceBase::XDMADeviceBase(const std::string& device)
    : device_(device), fd_(-1) {}

XDMADeviceBase::~XDMADeviceBase() {
    closeDevice();
}

bool XDMADeviceBase::initialize() {
    try {
        openDevice();
    } catch (const std::runtime_error& e) {
        std::cerr << "Error during initialization: " << e.what() << "\n";
        return false;
    }
    return true;
}

void XDMADeviceBase::openDevice() {
    if (isDeviceOpen()) {
        return;
    }
    fd_ = ::open(device_.c_str(), O_RDWR | O_NONBLOCK);
    if (fd_ < 0) {
        throw std::runtime_error("Failed to open device: " + std::string(std::strerror(errno)));
    }
}

void XDMADeviceBase::closeDevice() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

bool XDMADeviceBase::isDeviceOpen() const {
    return fd_ >= 0;
}
