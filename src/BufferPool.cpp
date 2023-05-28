//
// Created by Raziq on 2023-04-26.
//

#include "BufferPool.h"
#include <algorithm>
#include <cstring>
using namespace std;

BufferPool::BufferPool(size_t buffer_size)
        : buffer_size_(buffer_size), buffer_(new char[buffer_size]), read_pos_(0), write_pos_(0) {}

BufferPool::~BufferPool() {
    delete[] buffer_;
}

size_t BufferPool::read(void *data, size_t size) {
    size_t bytes_read = 0;
    while (size > 0 && read_pos_ < write_pos_) {
        size_t bytes_to_read = std::min(size, write_pos_ - read_pos_);
        std::memcpy(data, buffer_ + read_pos_, bytes_to_read);
        read_pos_ += bytes_to_read;
        size -= bytes_to_read;
        bytes_read += bytes_to_read;
    }
    return bytes_read;
}

size_t BufferPool::write(const void *data, size_t size) {
    size_t bytes_written = 0;
    while (size > 0) {
        size_t bytes_to_write = std::min(size, buffer_size_ - write_pos_);
        std::memcpy(buffer_ + write_pos_, data, bytes_to_write);
        write_pos_ += bytes_to_write;
        size -= bytes_to_write;
        bytes_written += bytes_to_write;
        if (write_pos_ == buffer_size_) {
            flush();
        }
    }
    return bytes_written;
}

void BufferPool::flush() {
    // Write buffer to disk
    write_pos_ = 0;
    read_pos_ = 0;
}