#include "xdma_device.h"
#include <iostream>

XDMADevice::XDMADevice(const std::string& readDevice, const std::string& writeDevice, const std::string& controlDevice)
    : readDevice_(readDevice), writeDevice_(writeDevice), controlDevice_(controlDevice) {}

XDMADevice::~XDMADevice() {
    // Clean up if needed
}

bool XDMADevice::initialize() {
    bool readInitialized = readDevice_.initialize();
    bool writeInitialized = writeDevice_.initialize();
    bool controlInitialized = controlDevice_.initialize();
    return readInitialized && writeInitialized && controlInitialized;
}

std::vector<char> XDMADevice::readFromDevice(uint32_t address, size_t size) {
    return readDevice_.readFromDevice(address, size);
}

bool XDMADevice::writeToDevice(uint32_t address, size_t size, const char* data) {
    return writeDevice_.writeToDevice(address, size, data);
}

bool XDMADevice::writeToDevice(uint32_t address, size_t size, const std::string& filename) {
    return writeDevice_.writeToDevice(address, size, filename);
}

void XDMADevice::printReadTransferSpeed() const {
    readDevice_.printTransferSpeed();
}

void XDMADevice::printWriteTransferSpeed() const {
    writeDevice_.printTransferSpeed();
}

void XDMADevice::printReadHexDump(size_t size) const {
    readDevice_.printHexDump(size);
}

// Implement additional control methods if needed
