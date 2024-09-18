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

bool XDMADeviceWrite::writeToDevice(uint32_t address, const std::string& filename) {
    // Open the file containing the data to write
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading.\n";
        return false;
    }

    // Get file size
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file data into a buffer
    std::vector<std::byte> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cerr << "Failed to read file data.\n";
        return false;
    }

    // Write the data to the device
    return writeToDevice(address, buffer);
}

bool XDMADeviceWrite::writeToDevice(uint32_t address, const void* data, size_t size) {
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

    // Update buffer size after write operation
    buffer_.resize(size);

    return true;
}

// New method for std::vector<std::byte>
bool XDMADeviceWrite::writeToDevice(uint32_t address, const std::vector<std::byte>& byteArray) {
    return writeToDevice(address, byteArray.data(), byteArray.size());
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
