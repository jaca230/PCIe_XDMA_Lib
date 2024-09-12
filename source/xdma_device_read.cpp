#include "xdma_device_read.h"
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <chrono>
#include <stdexcept>
#include <cstdlib>
#include <cassert>

XDMADeviceRead::XDMADeviceRead(const std::string& device)
    : XDMADeviceBase(device) {}

XDMADeviceRead::~XDMADeviceRead() {}

std::vector<char> XDMADeviceRead::readFromDevice(uint32_t address, size_t size) {
    if (!isDeviceOpen()) {
        std::cerr << "Device not open\n";
        return {}; // Return an empty vector
    }

    buffer_.resize(size);  // Resize buffer to the desired size
    off_t off = ::lseek(fd_, address, SEEK_SET);
    if (off < 0) {
        std::cerr << "Failed to seek to address: " << std::strerror(errno) << "\n";
        return {}; // Return an empty vector
    }
    std::fill(buffer_.begin(), buffer_.end(), 0x00); // Optionally fill buffer with zeros

    startTime_ = std::chrono::steady_clock::now();
    ssize_t bytes = ::read(fd_, buffer_.data(), size);
    endTime_ = std::chrono::steady_clock::now();

    if (bytes < 0) {
        std::cerr << "Error reading from device: " << std::strerror(errno) << "\n";
        return {}; // Return an empty vector
    } else if (bytes < static_cast<ssize_t>(size)) {
        std::cerr << "Short read of " << bytes << " bytes into a " << size << " bytes buffer\n";
        buffer_.resize(bytes); // Resize buffer to the actual number of bytes read
    }

    return buffer_; // Return the buffer with the data read
}

void XDMADeviceRead::printTransferSpeed() const {
    if (startTime_ == endTime_) {
        std::cerr << "No transfer performed\n";
        return;
    }
    std::chrono::duration<double> elapsed_seconds = endTime_ - startTime_;
    double transfer_speed = (buffer_.size() / (1024.0 * 1024.0)) / elapsed_seconds.count();
    std::cout << "Transfer speed: " << transfer_speed << " MB/s\n";
}

void XDMADeviceRead::printHexDump(size_t size) const {
    if (buffer_.empty()) {
        std::cerr << "No data to dump\n";
        return;
    }
    for (size_t i = 0; i < size; i += 16) {
        std::cout << std::hex << i << ": ";
        for (size_t j = 0; j < 16 && i + j < size; ++j) {
            std::cout << std::hex << static_cast<unsigned>(buffer_[i + j]) << " ";
        }
        std::cout << " |";
        for (size_t j = 0; j < 16 && i + j < size; ++j) {
            char c = buffer_[i + j];
            if (c >= 32 && c <= 126) {
                std::cout << c;
            } else {
                std::cout << ".";
            }
        }
        std::cout << "|\n";
    }
}
