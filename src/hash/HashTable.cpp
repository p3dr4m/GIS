//HashTable.cpp
#include "HashTable.h"

using namespace std;

void HashTable::insert(string featureName, string stateAbbreviation, int offset) {
    int index = hash(featureName, stateAbbreviation);
    HashEntry entry;
    entry.featureName = featureName;
    entry.stateAbbreviation = stateAbbreviation;
    entry.offsets.push_back(offset);
    entry.exists = true;
    int tempProbe = 1;

    for (int i = 0; i < capacity; i++) {
        if (!data[index].exists) {
            data[index] = entry;
            size++;
            break;
        } else if (data[index].featureName == featureName && data[index].stateAbbreviation == stateAbbreviation) {
            data[index].offsets.push_back(offset);
            break;
        } else {
            index = (index + i * i) % capacity;
            tempProbe++;
            if (tempProbe > longestProbe) {
                longestProbe = tempProbe;
            }
        }
    }

    if (((size * 1.0) / capacity) >= loadFactor) {
        resize();
    }
}

/**
 * @brief we use elf hash function to hash the key
 * @param featureName
 * @param stateAbbreviation
 * @return
 */
unsigned int HashTable::hash(const string &featureName, const string &stateAbbreviation) const {
    string key = featureName + stateAbbreviation;
    unsigned int hash = 0;
    unsigned int x = 0;
    unsigned int i = 0;
    unsigned int len = key.length();

    for (i = 0; i < len; i++) {
        hash = (hash << 4) + (key[i]);
        if ((x = hash & 0xF0000000) != 0) {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }

    return hash % capacity;
}

void HashTable::resize() {
    capacity *= 2;
    vector<HashEntry> temp = data;
    data.clear();
    data.resize(capacity);
    size = 0;
    for (auto &entry: temp) {
        if (entry.exists) {
            insert(entry.featureName, entry.stateAbbreviation, entry.offsets[0]);
        }
    }
}

vector<int> HashTable::find(const string &featureName, const string &stateAbbreviation) {
    unsigned int index = hash(featureName, stateAbbreviation);

    for (int i = 0; i < capacity; i++) {
        if ((data[index].featureName.find(featureName) != string::npos) && data[index].stateAbbreviation == stateAbbreviation) {
            return data[index].offsets;
        } else {
            index = (index + i * i) % capacity;
        }
    }

    return {};
}

int HashTable::getLongestProbe() {
    return longestProbe;
}

string HashTable::str() {
    // show the hash table in a string
    string result;
    for (int i = 0; i < capacity; i++) {
        if (data[i].exists) {
            string offsetStr = "";

            for (int offset : data[i].offsets) {
                offsetStr += to_string(offset);
                if (data[i].offsets.size() > 1) {
                    offsetStr += ", ";
                } else {
                    offsetStr += "";
                }
            }

            result += "\t" + to_string(i) + ": [" + data[i].featureName + " " + data[i].stateAbbreviation + ", [" + offsetStr +
                      "]]\n";
        }
    }
    return result;
}
