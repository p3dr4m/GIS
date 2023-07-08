#ifndef INC_8042_PROJECT_BUFFERPOOL_H
#define INC_8042_PROJECT_BUFFERPOOL_H

#include <list>
#include <unordered_map>
#include <sstream>


// https://www.geeksforgeeks.org/lru-cache-implementation/ used for help

template<typename T>
class BufferPool {
public:
    explicit BufferPool(size_t size) : maxSize(size) {}

    void put(const T &item) {
        auto key = item.offset;

        // if item exists, remove from buffer
        if (cacheMap.find(key) != cacheMap.end()) {
            cacheList.erase(cacheMap[key]);
            cacheMap.erase(key);
        }

        // If buffer full, remove LRU item
        if (cacheList.size() == maxSize) {
            T lastItem = cacheList.back();
            cacheList.pop_back();
            cacheMap.erase(lastItem.offset);
        }

        // Insert the item at the front of the cache
        cacheList.push_front(item);
        cacheMap[key] = cacheList.begin();
    }

    T get(const int key) {
        if (cacheMap.find(key) == cacheMap.end()) {
            throw std::out_of_range("Key not found in BufferPool");
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

    std::string str() const {
        std::stringstream ss;
        ss << "MRU\n";
        for (const auto &item: cacheList) {
            ss << "  " << item.offset << ": " << item.line << "\n";
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
