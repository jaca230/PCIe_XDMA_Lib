#include "xdma_device.h"
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <chrono>
#include <stdexcept>

XDMADevice::XDMADevice(const std::string& device)
    : device_(device), fd_(-1) {}

XDMADevice::~XDMADevice() {
    closeDevice();
}

bool XDMADevice::initialize() {
    try {
        openDevice();
    } catch (const std::runtime_error& e) {
        std::cerr << "Error during initialization: " << e.what() << "\n";
        return false;
    }
    return true;
}

void XDMADevice::openDevice() {
    fd_ = ::open(device_.c_str(), O_RDWR | O_NONBLOCK);
    if (fd_ < 0) {
        throw std::runtime_error("Failed to open device: " + std::string(std::strerror(errno)));
    }
}

void XDMADevice::closeDevice() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

ssize_t XDMADevice::readFromDevice(uint32_t address, size_t size) {
    // Ensure the buffer can hold the requested size
    buffer_.resize(size);

    // Seek to the given address
    off_t off = ::lseek(fd_, address, SEEK_SET);
    if (off < 0) {
        std::cerr << "Failed to seek to address: " << std::strerror(errno) << "\n";
        return -1;
    }

    // Clear the buffer
    std::fill(buffer_.begin(), buffer_.end(), 0x00);

    // Start the timer
    startTime_ = std::chrono::steady_clock::now();

    // Read from the device
    ssize_t bytes = ::read(fd_, buffer_.data(), size);

    // End the timer
    endTime_ = std::chrono::steady_clock::now();

    if (bytes < 0) {
        std::cerr << "Error reading device: " << std::strerror(errno) << "\n";
    } else if (bytes < static_cast<ssize_t>(size)) {
        std::cerr << "Short read of " << bytes << " bytes into a " << size << " bytes buffer\n";
    }

    return bytes;
}

void XDMADevice::printTransferSpeed() const {
    std::chrono::duration<double> elapsed_seconds = endTime_ - startTime_;
    double transfer_speed = (buffer_.size() / (1024.0 * 1024.0)) / elapsed_seconds.count();
    std::cout << "Transfer speed: " << transfer_speed << " MB/s\n";
}

void XDMADevice::printHexDump(size_t size) const {
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
