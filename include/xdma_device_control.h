#ifndef XDMA_DEVICE_CONTROL_H
#define XDMA_DEVICE_CONTROL_H

#include "xdma_device_base.h"
#include <map>
#include <cstdint>

class XDMADeviceControl : public XDMADeviceBase {
public:
    XDMADeviceControl(const std::string& device);
    ~XDMADeviceControl();

    int getNumberOfH2CChannels();
    int getNumberOfC2HChannels();
    bool isAnyChannelStreaming() const;
    bool isChannelEnabled(bool isH2C, int channelIndex) const;
    bool isChannelStreaming(bool isH2C, int channelIndex) const;

private:
    struct ChannelStatus {
        bool enabled;
        bool streaming;
    };

    std::map<int, ChannelStatus> h2cChannelStatuses;
    std::map<int, ChannelStatus> c2hChannelStatuses;

    int readFromAddress(off_t address) const;

    // Constants
    static const int NUM_CHANNELS = 4;
    static const off_t H2C_BASE_ADDRESS = 0x0000;
    static const off_t C2H_BASE_ADDRESS = 0x1000;
    static const uint32_t CHANNEL_ID_MASK = 0xFFF;
    static const uint32_t STREAM_ENABLE_MASK = 0xF;
    static const uint32_t STREAM_ENABLE_VALUE = 0x8;
    static const uint32_t EXPECTED_CHANNEL_ID = 0x1FC;
    static const uint32_t CHANNEL_ADDRESS_OFFSET_INCREMENT = 0x100;
    static const int STREAM_ENABLE_BIT_SHIFT = 12;
    static const int CHANNEL_ID_BIT_SHIFT = 20;
    static const uint32_t MAP_SIZE = 32 * 1024UL;
    static const uint32_t MAP_MASK = MAP_SIZE - 1;
};

#endif // XDMA_DEVICE_CONTROL_H
