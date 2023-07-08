//HashTable.cpp
#include "HashTable.h"


// https://www.geeksforgeeks.org/quadratic-probing-in-hashing/
// https://www.geeksforgeeks.org/what-are-hash-functions-and-how-to-choose-a-good-hash-function/

using namespace std;

bool HashTable::insert(const string &featureName, const string &stateAbbreviation, int offset) {
    unsigned int index = hash(featureName, stateAbbreviation);
    HashEntry entry;
    entry.featureName = featureName;
    entry.stateAbbreviation = stateAbbreviation;
    entry.offsets.push_back(offset);
    entry.exists = true;
    bool inserted = false;

    for (int i = 0; i < capacity; i++) {
        if (!data[index].exists) {
            data[index] = entry;
            size++;
            inserted = true;
            break;
        } else if (data[index].featureName == featureName && data[index].stateAbbreviation == stateAbbreviation) {
            data[index].offsets.push_back(offset);
            inserted = true;
            break;
        } else {
            index = (index + i * i) % capacity;
        }
    }

    if (((size * 1.0) / capacity) >= loadFactor) {
        resize();
    }
    return inserted;
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

    for (int i = 0; i < capacity; i++) {
        index = (originalIndex + i * i) % capacity;
        bool hasFeatureName = data[index].featureName == featureName;
//        bool hasFeatureName = data[index].featureName.find(featureName) == std::string::npos;
        // If the existing entry matches the key
        if (data[index].exists &&
            hasFeatureName &&
            data[index].stateAbbreviation == stateAbbreviation) {
            return data[index].offsets;
        }
    }

    // If the key is not found
    return {};
}


string HashTable::str() {
    // show the hash table in a string
    string result;
    for (int i = 0; i < capacity; i++) {
        if (data[i].exists) {
            string offsetStr;

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

int HashTable::getLongestProbe() {
    int longestSequence = 0;
    int currentSequence = 0;
    for (int i = 0; i < capacity; i++) {
        if (data[i].exists) {
            currentSequence++;
        } else {
            if (currentSequence > longestSequence) {
                longestSequence = currentSequence;
            }
            currentSequence = 0;
        }
    }
    return longestSequence;
}
