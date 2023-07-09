//HashTable.h
#ifndef INC_8042_PROJECT_HASHTABLE_H
#define INC_8042_PROJECT_HASHTABLE_H

#include <string>
#include <vector>
#include "../structs/HashEntry.h"

class HashTable {
public:

    explicit HashTable(int _capacity) : size{0}, capacity{_capacity} {
        data.resize(capacity);
    }

    ~HashTable() = default;

    bool insert(const std::string &featureName, const std::string &stateAbbreviation, int offset);

    std::vector<int> find(const std::string &featureName, const std::string &stateAbbreviation);

    int getLongestProbe();


    std::string str();

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }


private:
    std::vector<HashEntry> data;
    int size; // current number of elements in the hash table
    int capacity; // max capacity of the hash table
    float loadFactor = 0.7;

    unsigned int hash(const std::string &featureName, const std::string &stateAbbreviation) const;

    void resize();

    static bool isPrime(int n) {
        if (n == 2 || n == 3)
            return true;

        if (n == 1 || n % 2 == 0)
            return false;

        for (int i = 3; i * i <= n; i += 2)
            if (n % i == 0)
                return false;

        return true;
    }

    static int nextPrime(int n) {
        if (n % 2 == 0)
            ++n;

        for (; !isPrime(n); n += 2);

        return n;
    }
};

#endif //INC_8042_PROJECT_HASHTABLE_H
