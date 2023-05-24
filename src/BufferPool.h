//
// Created by Raziq on 2023-04-26.
//

#ifndef FINALPROJALGO_BUFFERPOOL_H
#define FINALPROJALGO_BUFFERPOOL_H
#include <cstdlib>

class BufferPool {
public:
  explicit BufferPool(size_t buffer_size);
  ~BufferPool();

  size_t read(void* data, size_t size);
  size_t write(const void* data, size_t size);
  void flush();

private:
  const size_t buffer_size_;
  char* buffer_;
  size_t read_pos_;
  size_t write_pos_;
};


#endif //FINALPROJALGO_BUFFERPOOL_H
