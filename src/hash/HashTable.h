//HashTable.h
#ifndef INC_8042_PROJECT_HASHTABLE_H
#define INC_8042_PROJECT_HASHTABLE_H

#include <string>
#include <vector>

struct HashEntry {
    std::string featureName;
    std::string stateAbbreviation;
    std::vector<int> offsets;
    bool exists = false;
};

class HashTable {
public:

    explicit HashTable(int _capacity) : size{0}, capacity{_capacity} {
        data.resize(capacity);
    }

    ~HashTable() = default;

    void insert(const std::string &featureName, const std::string &stateAbbreviation, int offset);

    std::vector<int> find(const std::string &featureName, const std::string &stateAbbreviation);

    int getLongestProbe() const {
        return longestProbe;
    }

    std::string str();

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }

private:
    std::vector<HashEntry> data;
    int longestProbe = 1;
    int size; // current number of elements in the hash table
    int capacity; // max capacity of the hash table
    float loadFactor = 0.7;

    unsigned int hash(const std::string &featureName, const std::string &stateAbbreviation) const;

    void resize();

};

#endif //INC_8042_PROJECT_HASHTABLE_H
