// Starter code for lab 05 of COMP 8042
// You can compile and run this file using the following command:
//			g++ -std=c++11 -pthread hashtable.cc libgtest.a
#include <random>
#include <iostream>
#include <gtest/gtest.h>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

template <typename K>
class HashFunction {
public:
    virtual unsigned int operator()(const K& key) const = 0;
    virtual ~HashFunction() = default;
};

class SimpleStringHash : public HashFunction<string> {
public:
    // Computes a simple hash value for a string
    unsigned int operator()(const string& s) const {
        unsigned int hash = 0;
        for (int i = 0; i < s.size(); i++) {
            hash += s[i];
        }
        return hash;
    }
};

/// An abstract collision resolution function
class ResolutionFunction {
public:
    virtual unsigned int operator()(int i) const = 0;
    virtual ~ResolutionFunction() = default;
};

/// The instructor did the hard part for you and implemented the two resolution functions. Phew!
class LinearProbing : public ResolutionFunction {
public:
    unsigned int operator()(int i) const {
        return i;
    }
};

class QuadraticProbing : public ResolutionFunction {
public:
    unsigned int operator()(int i) const {
        return i * i;
    }
};

/// Returns the bit index of the most significant bit.
/// If the input is zero, it returns zero
/// This function will help you locate the crrect good prime in the array below
/// It will also help you compute the next power of two
int mostSignificantBit(int x) {
    if (x == 0)
        return 0;
    int mostSignificantBit = 31;
    while ((x & (1 << mostSignificantBit)) == 0)
        mostSignificantBit--;
    return mostSignificantBit;
}

/// Good prime numbers to use as Hashtable sizes
/// Copied from https://web.archive.org/web/20120705020114/http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
int GoodPrimeNumbers[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
    24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469,
    12582917, 25165843, 50331653, 100663319, 201326611, 402653189,
    805306457, 1610612741
};


template <typename K>
class Hashtable{
    enum BucketStatus {EMPTY, OCCUPIED, DELETED};
    
    /// The total number of collisions since the creation of the hashfunction.
    /// This includes the collisions encountered during insertions, searches,
    /// and deletions
    int numCollisions;
    
    /// All the buckets in the hashtable
    // TODO define a list of buckets of type K
    
    /// Status of all the buckets
    // TODO define a list of status
    
    /// The hash function
    HashFunction<K>* hash;
    
    /// The collision resolution function
    ResolutionFunction* f;
public:
    Hashtable(int n, HashFunction<K>* _h, ResolutionFunction* _f) : numCollisions(0), hash(_h), f(_f) {
        int c = n * 15 / 10; // Initialize to 150% of the maximum size. This is a bad choice; change it!
        // TODO initialize the buckets and status to the given capacity
    }
    
    void insert(const K& key) {
        // TODO implement the insert function
    }
    
    bool search(const K& key) {
        // TODO implement the search function
        return false;
    }
    
    bool erase(const K& key) {
        // TODO implement the delete function
        return false;
    }
    
    int getNumCollisions() const {
        return numCollisions;
    }
};

/// The following code is only for testing. Any changes you make
/// beyond this point will be ignored in the final deliverable

vector<string> loadWords(string filename) {
    ifstream file(filename);
    vector<string> lines;
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            // Remove trailing newlines
            while (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n')
                line = line.substr(0, line.size() - 1);
            lines.push_back(line);
        }
    }
    return lines;
}

void generateRandomPhrases(const vector<string>& words, int numPhrases, vector<string>& phrases) {
    srand(0);
    for (int i = 0; i < numPhrases; i++) {
        string line;
        int length = rand() % 4 + 2; // Generate a random number in [2, 5]
        for (int iWord = 0; iWord < length; iWord++) {
            if (iWord != 0)
                line += " ";
            string word = words[rand() % words.size()];
            line = line + word;
        }
        phrases.push_back(line);
    }
    sort(phrases.begin(), phrases.end());
    int newSize = unique(phrases.begin(), phrases.end()) - phrases.begin();
    phrases.resize(newSize);
    shuffle (phrases.begin(), phrases.end(), std::default_random_engine(0));
}

void testHashtableSimple() {
    HashFunction<string>* hash = new SimpleStringHash();
    ResolutionFunction* f = new LinearProbing();
    Hashtable<string> hashtable = Hashtable<string>(10, hash, f);
    hashtable.insert("first word");
    hashtable.insert("second word");
    hashtable.insert("third word");
    EXPECT_TRUE(hashtable.search("first word"));
    EXPECT_TRUE(hashtable.search("second word"));
    EXPECT_TRUE(hashtable.search("third word"));
    EXPECT_FALSE(hashtable.search("fourth word"));
    EXPECT_TRUE(hashtable.erase("first word"));
    EXPECT_FALSE(hashtable.search("first word"));
    EXPECT_TRUE(hashtable.search("second word"));
    EXPECT_TRUE(hashtable.search("third word"));
    
    delete hash;
    delete f;
}

int stressTest(int hashtableSize) {
    // Load words from file
    vector<string> words = loadWords("1kwords.txt");
    // Generate random phrases
    vector<string> phrases;
    generateRandomPhrases(words, hashtableSize, phrases);

    // Create the hashtable
    HashFunction<string>* hash = new SimpleStringHash();
    //ResolutionFunction* f = new LinearProbing();
    ResolutionFunction* f = new QuadraticProbing();
    Hashtable<string> hashtable = Hashtable<string>(hashtableSize, hash, f);

    
    // Insert all phrases into the hashtable
    for (string phrase : phrases)
        hashtable.insert(phrase);
    // Search for all phrases
    for (string phrase : phrases)
        hashtable.search(phrase);
        
    delete hash;
    delete f;
    // return number of collisions
    return hashtable.getNumCollisions();
}

int main() {
    testHashtableSimple();
    int sizes[] = {100, 1000, 10000, 100000};
    for (int hashtableSize : sizes) {
        int numCollisions = stressTest(hashtableSize);
        cout << "Number of collisions with " << hashtableSize << " phrases is " << numCollisions << endl;
    }
	return 0;
}
