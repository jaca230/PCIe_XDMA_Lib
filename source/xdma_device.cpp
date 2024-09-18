#include "xdma_device.h"
#include <iostream>

// Static constant definitions
const std::string XDMADevice::CONTROL_DEVICE_SUFFIX = "_control";
const std::string XDMADevice::READ_DEVICE_SUFFIX = "_c2h_";
const std::string XDMADevice::WRITE_DEVICE_SUFFIX = "_h2c_";
const int XDMADevice::INVALID_CHANNEL_INDEX = -1;

XDMADevice::XDMADevice(const std::string& devicePrefix)
    : devicePrefix_(devicePrefix),
      controlDevice_(devicePrefix_ + CONTROL_DEVICE_SUFFIX) {}

XDMADevice::~XDMADevice() {
    // Clean up if needed
}

bool XDMADevice::initialize() {
    bool controlInitialized = controlDevice_.initialize();

    if (!controlInitialized) {
        std::cerr << "Failed to initialize control device\n";
        return false;
    }

    int numH2CChannels = controlDevice_.getNumberOfH2CChannels();
    int numC2HChannels = controlDevice_.getNumberOfC2HChannels();

    for (int i = 0; i < numC2HChannels; ++i) {
        readDevices_.emplace_back(devicePrefix_ + READ_DEVICE_SUFFIX + std::to_string(i));
        if (!readDevices_.back().initialize()) {
            std::cerr << "Failed to initialize read device for channel " << i << "\n";
            return false;
        }
    }

    for (int i = 0; i < numH2CChannels; ++i) {
        writeDevices_.emplace_back(devicePrefix_ + WRITE_DEVICE_SUFFIX + std::to_string(i));
        if (!writeDevices_.back().initialize()) {
            std::cerr << "Failed to initialize write device for channel " << i << "\n";
            return false;
        }
    }

    return true;
}

std::vector<char> XDMADevice::readFromDevice(uint32_t address, size_t size, int channelIndex) {
    if (channelIndex >= 0 && channelIndex < readDevices_.size()) {
        return readDevices_[channelIndex].readFromDevice(address, size);
    } else {
        std::cerr << "Invalid channel index for read operation.\n";
        return {};
    }
}

bool XDMADevice::writeToDevice(uint32_t address, size_t size, const char* data, int channelIndex) {
    if (channelIndex >= 0 && channelIndex < writeDevices_.size()) {
        return writeDevices_[channelIndex].writeToDevice(address, size, data);
    } else {
        std::cerr << "Invalid channel index for write operation.\n";
        return false;
    }
}

bool XDMADevice::writeToDevice(uint32_t address, size_t size, const std::string& filename, int channelIndex) {
    if (channelIndex >= 0 && channelIndex < writeDevices_.size()) {
        return writeDevices_[channelIndex].writeToDevice(address, size, filename);
    } else {
        std::cerr << "Invalid channel index for write operation.\n";
        return false;
    }
}

void XDMADevice::printReadTransferSpeed(int channelIndex) const {
    if (channelIndex >= 0 && channelIndex < readDevices_.size()) {
        readDevices_[channelIndex].printTransferSpeed();
    } else {
        std::cerr << "Invalid channel index for read transfer speed.\n";
    }
}

void XDMADevice::printWriteTransferSpeed(int channelIndex) const {
    if (channelIndex >= 0 && channelIndex < writeDevices_.size()) {
        writeDevices_[channelIndex].printTransferSpeed();
    } else {
        std::cerr << "Invalid channel index for write transfer speed.\n";
    }
}

void XDMADevice::printReadHexDump(size_t size, int channelIndex) const {
    if (channelIndex >= 0 && channelIndex < readDevices_.size()) {
        readDevices_[channelIndex].printHexDump(size);
    } else {
        std::cerr << "Invalid channel index for read hex dump.\n";
    }
}

int XDMADevice::getH2CChannelCount() {
    return controlDevice_.getNumberOfH2CChannels();
}

int XDMADevice::getC2HChannelCount() {
    return controlDevice_.getNumberOfC2HChannels();
}

bool XDMADevice::isStreaming() const {
    return controlDevice_.isAnyChannelStreaming();
}
