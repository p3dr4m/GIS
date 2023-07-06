#ifndef INC_8042_PROJECT_BUFFERPOOL_H
#define INC_8042_PROJECT_BUFFERPOOL_H

#include <list>
#include <unordered_map>
#include <stdexcept>
#include <sstream>

template <typename T>
class BufferPool {
public:
    explicit BufferPool(size_t size): maxSize(size) {}

    void put(const T& item) {
        auto key = item.getKey();

        // If item already in buffer, remove it first
        if (cacheMap.find(key) != cacheMap.end()) {
            cacheList.erase(cacheMap[key]);
            cacheMap.erase(key);
        }

        // If the buffer is full, remove the least recently used item
        if (cacheList.size() == maxSize) {
            T lastItem = cacheList.back();
            cacheList.pop_back();
            cacheMap.erase(lastItem.getKey());
        }

        // Insert the item at the front of the cache
        cacheList.push_front(item);
        cacheMap[key] = cacheList.begin();
    }

    T get(const int key) {
        if (cacheMap.find(key) == cacheMap.end()) {
            throw std::out_of_range("Key not found in LRUBufferPool");
        }

        // Copy the accessed item
        T item = *cacheMap[key];

        // Erase the accessed item from the list and the map
        cacheList.erase(cacheMap[key]);
        cacheMap.erase(key);

        // Insert the copied item to the front of the list
        cacheList.push_front(item);
        cacheMap[key] = cacheList.begin();

        return item;
    }

    bool exists(const int key) const {
        return cacheMap.find(key) != cacheMap.end();
    }

    size_t size() const {
        return cacheMap.size();
    }

    std::string str() const {
        std::stringstream ss;
        ss << "MRU\n";
        for(const auto& item : cacheList) {
            ss << "  " <<item.getKey() << ": " << item.str() << "\n";
        }
        ss << "LRU\n";
        return ss.str();
    }

private:
    std::list<T> cacheList;
    std::unordered_map<int, typename std::list<T>::iterator> cacheMap;
    size_t maxSize;
};



#endif //INC_8042_PROJECT_BUFFERPOOL_H
