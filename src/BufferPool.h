
#ifndef INC_8042_PROJECT_BUFFERPOOL_H
#define INC_8042_PROJECT_BUFFERPOOL_H

#include <cstdlib>

class BufferPool {
public:
    explicit BufferPool(size_t buffer_size);

    ~BufferPool();

    size_t read(void *data, size_t size);

    size_t write(const void *data, size_t size);

    void flush();

private:
    const size_t buffer_size_;
    char *buffer_;
    size_t read_pos_;
    size_t write_pos_;
};


#endif //INC_8042_PROJECT_BUFFERPOOL_H
