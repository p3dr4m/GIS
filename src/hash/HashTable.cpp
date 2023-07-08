//HashTable.cpp
#include "HashTable.h"

using namespace std;

void HashTable::insert(const string &featureName, const string &stateAbbreviation, int offset) {
    unsigned int index = hash(featureName, stateAbbreviation);
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

unsigned int HashTable::hash(const string &featureName, const string &stateAbbreviation) const {
    // djb2 hash http://www.cse.yorku.ca/~oz/hash.html
    string key = featureName + stateAbbreviation;
    unsigned int hash = 5381;
    unsigned int i = 0;
    unsigned int len = key.length();

    for (i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + key[i];
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
    unsigned int originalIndex = index;
    int i = 0;

    while (data[index].exists &&
           (data[index].featureName != featureName || data[index].stateAbbreviation != stateAbbreviation)) {
        i++;
        index = (originalIndex + i * i) % capacity;

        // If we have checked all entries, break
        if (i == capacity) {
            return {};
        }
    }

    // If the existing entry matches the key
    if (data[index].exists) {
        return data[index].offsets;
    }

    // If the key is not found
    return {};
}


string HashTable::str() {
    // show the hash table in a string
    string result;
    for (int i = 0; i < capacity; i++) {
        if (data[i].exists) {
            string offsetStr = "";

            for (int offset: data[i].offsets) {
                offsetStr += to_string(offset);
                if (data[i].offsets.size() > 1) {
                    offsetStr += ", ";
                } else {
                    offsetStr += "";
                }
            }

            result += "\t" + to_string(i) + ": [" + data[i].featureName + " " + data[i].stateAbbreviation + ", [" +
                      offsetStr +
                      "]]\n";
        }
    }
    return result;
}

