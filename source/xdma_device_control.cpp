#include "xdma_device_control.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <cerrno>

XDMADeviceControl::XDMADeviceControl(const std::string& device)
    : XDMADeviceBase(device) {}

XDMADeviceControl::~XDMADeviceControl() {}

int XDMADeviceControl::getNumberOfH2CChannels() {
    h2cChannelStatuses.clear();  // Clear previous statuses
    int h2cChannels = 0;
    for (int i = 0; i < NUM_CHANNELS; i++) {
        uint32_t statusRegVal = readFromAddress(H2C_BASE_ADDRESS + (i * CHANNEL_ADDRESS_OFFSET_INCREMENT));
        uint32_t channelId = (statusRegVal >> CHANNEL_ID_BIT_SHIFT) & CHANNEL_ID_MASK;
        uint32_t streamEnable = (statusRegVal >> STREAM_ENABLE_BIT_SHIFT) & STREAM_ENABLE_MASK;

        bool enabled = (channelId == EXPECTED_CHANNEL_ID);
        bool streaming = (streamEnable == STREAM_ENABLE_VALUE);

        h2cChannelStatuses[i] = {enabled, streaming};

        if (enabled) {
            h2cChannels++;
        }
    }
    return h2cChannels;
}

int XDMADeviceControl::getNumberOfC2HChannels() {
    c2hChannelStatuses.clear();  // Clear previous statuses
    int c2hChannels = 0;
    for (int i = 0; i < NUM_CHANNELS; i++) {
        uint32_t statusRegVal = readFromAddress(C2H_BASE_ADDRESS + (i * CHANNEL_ADDRESS_OFFSET_INCREMENT));
        uint32_t channelId = (statusRegVal >> CHANNEL_ID_BIT_SHIFT) & CHANNEL_ID_MASK;
        uint32_t streamEnable = (statusRegVal >> STREAM_ENABLE_BIT_SHIFT) & STREAM_ENABLE_MASK;

        bool enabled = (channelId == EXPECTED_CHANNEL_ID);
        bool streaming = (streamEnable == STREAM_ENABLE_VALUE);

        c2hChannelStatuses[i] = {enabled, streaming};

        if (enabled) {
            c2hChannels++;
        }
    }
    return c2hChannels;
}

bool XDMADeviceControl::isAnyChannelStreaming() const {
    // Check if any channel is streaming
    for (std::map<int, ChannelStatus>::const_iterator it = h2cChannelStatuses.begin(); it != h2cChannelStatuses.end(); ++it) {
        if (it->second.streaming) {
            return true;
        }
    }
    for (std::map<int, ChannelStatus>::const_iterator it = c2hChannelStatuses.begin(); it != c2hChannelStatuses.end(); ++it) {
        if (it->second.streaming) {
            return true;
        }
    }
    return false;
}

bool XDMADeviceControl::isChannelEnabled(bool isH2C, int channelIndex) const {
    if (isH2C) {
        auto it = h2cChannelStatuses.find(channelIndex);
        return it != h2cChannelStatuses.end() ? it->second.enabled : false;
    } else {
        auto it = c2hChannelStatuses.find(channelIndex);
        return it != c2hChannelStatuses.end() ? it->second.enabled : false;
    }
}

bool XDMADeviceControl::isChannelStreaming(bool isH2C, int channelIndex) const {
    if (isH2C) {
        auto it = h2cChannelStatuses.find(channelIndex);
        return it != h2cChannelStatuses.end() ? it->second.streaming : false;
    } else {
        auto it = c2hChannelStatuses.find(channelIndex);
        return it != c2hChannelStatuses.end() ? it->second.streaming : false;
    }
}

int XDMADeviceControl::readFromAddress(off_t address) const {
    if (!isDeviceOpen()) {
        throw std::runtime_error("Device not open");
    }

    void* map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    if (map_base == MAP_FAILED) {
        throw std::runtime_error("Failed to mmap device: " + std::string(strerror(errno)));
    }

    void* virt_addr = (char*)map_base + (address & MAP_MASK);
    uint32_t read_result = *((uint32_t*)virt_addr);

    if (munmap(map_base, MAP_SIZE) == -1) {
        throw std::runtime_error("Failed to munmap device: " + std::string(strerror(errno)));
    }

    return read_result;
}
