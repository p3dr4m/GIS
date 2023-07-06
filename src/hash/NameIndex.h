//NameIndex.h
#ifndef INC_8042_PROJECT_NAMEINDEX_H
#define INC_8042_PROJECT_NAMEINDEX_H

#include <utility>

#include "HashTable.h"
//#include "../GISRecord.h"

class NameIndex {
public:
    NameIndex() : hashTable(1024) {};

    ~NameIndex() = default;

    void insert(const std::string &featureName, const std::string &stateAbbreviation, int offset) {
        hashTable.insert(featureName, stateAbbreviation, offset);
    }

    std::vector<int> find(const std::string &featureName, const std::string &stateAbbreviation) {
        return hashTable.find(featureName, stateAbbreviation);
    }

    int getLongestProbe() {
        return hashTable.getLongestProbe();
    }

    std::string str() {
        return hashTable.str();
    }

    int getSize() {
        return hashTable.getSize();
    }



private:
    HashTable hashTable;
};


#endif //INC_8042_PROJECT_NAMEINDEX_H
