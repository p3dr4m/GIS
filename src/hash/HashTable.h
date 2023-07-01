//HashTable.h
#ifndef INC_8042_PROJECT_HASHTABLE_H
#define INC_8042_PROJECT_HASHTABLE_H

#include <string>
#include <vector>

// create a hash table with fixed size 1024
// the hash table must use quadratic probing to resolve collisions
// the table will double in size when the load factor is 0.7
// the hash table will store a vector

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

    void insert(std::string featureName, std::string stateAbbreviation, int offset);

    std::vector<int> find(const std::string &featureName, const std::string &stateAbbreviation);

    int getProbeCount();

    std::string str();

private:
    std::vector<HashEntry> data;
    int longestProbe = 1;
    int size; // current number of elements in the hash table
    int capacity; // max capacity of the hash table
    float loadFactor = 0.7;

    int hash(const std::string &featureName, const std::string &stateAbbreviation);

    void resize();

};

#endif //INC_8042_PROJECT_HASHTABLE_H
