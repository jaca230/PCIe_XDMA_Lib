#include "xdma_device_write.h"
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <chrono>
#include <unistd.h>

XDMADeviceWrite::XDMADeviceWrite(const std::string& device)
    : XDMADeviceBase(device) {}

XDMADeviceWrite::~XDMADeviceWrite() {}

bool XDMADeviceWrite::writeToDevice(uint32_t address, size_t size, const std::string& filename) {
    buffer_.resize(size);

    if (!filename.empty()) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open file: " << std::strerror(errno) << "\n";
            return false;
        }
        file.read(buffer_.data(), size);
        if (file.gcount() != static_cast<std::streamsize>(size)) {
            std::cerr << "Failed to read the correct amount of data from file\n";
            return false;
        }
    } else {
        std::cerr << "No filename provided for write operation.\n";
        return false;
    }

    return writeToDevice(address, size, buffer_.data());
}

bool XDMADeviceWrite::writeToDevice(uint32_t address, size_t size, const char* data) {
    if (!isDeviceOpen()) {
        std::cerr << "Device not open\n";
        return false;
    }

    off_t off = ::lseek(fd_, address, SEEK_SET);
    if (off < 0) {
        std::cerr << "Failed to seek to address: " << std::strerror(errno) << "\n";
        return false;
    }

    startTime_ = std::chrono::steady_clock::now();
    ssize_t bytes_written = ::write(fd_, data, size);
    endTime_ = std::chrono::steady_clock::now();

    if (bytes_written < 0) {
        std::cerr << "Error writing to device: " << std::strerror(errno) << "\n";
        return false;
    } else if (bytes_written < static_cast<ssize_t>(size)) {
        std::cerr << "Short write of " << bytes_written << " bytes to device\n";
    }

    return true;
}

void XDMADeviceWrite::printTransferSpeed() const {
    if (startTime_ == endTime_) {
        std::cerr << "No transfer performed\n";
        return;
    }
    std::chrono::duration<double> elapsed_seconds = endTime_ - startTime_;
    double transfer_speed = (buffer_.size() / (1024.0 * 1024.0)) / elapsed_seconds.count();
    std::cout << "Transfer speed: " << transfer_speed << " MB/s\n";
}
