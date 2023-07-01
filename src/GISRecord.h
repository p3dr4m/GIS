#ifndef INC_8042_PROJECT_GISRECORD_H
#define INC_8042_PROJECT_GISRECORD_H

#include <string>
#include "quad/CoordinateIndex.h"
#include "hash/NameIndex.h"
#include "BufferPool.h"
#include <stdexcept>
#include <utility>


struct DMS {
    int degrees;
    int minutes;
    int seconds;
    char direction;

    DMS() = default;


    explicit DMS(const std::string &dms);

    DMS(float point) {
        degrees = static_cast<int>(point);
        minutes = static_cast<int>((point - degrees) * 60);
        seconds = static_cast<int>((point - degrees - minutes / 60.0) * 3600);
    }

    std::string toLogString() const {
        std::string logDirection;
        if (direction == 'N') {
            logDirection = "North";
        } else if (direction == 'S') {
            logDirection = "South";
        } else if (direction == 'E') {
            logDirection = "East";
        } else if (direction == 'W') {
            logDirection = "West";
        }
        return std::to_string(degrees) + "d " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s " +
               logDirection;
    }

    std::string toString() const;

    std::string toTotalSeconds() const;

    float toFloat() const;
};

enum RecordHeader {
    FEATURE_ID = 0,
    FEATURE_NAME,
    FEATURE_CLASS,
    STATE_ALPHA,
    STATE_NUMERIC,
    COUNTY_NAME,
    COUNTY_NUMERIC,
    PRIMARY_LAT_DMS,
    PRIM_LONG_DMS,
    PRIM_LAT_DEC,
    PRIM_LONG_DEC,
    SOURCE_LAT_DMS,
    SOURCE_LONG_DMS,
    SOURCE_LAT_DEC,
    SOURCE_LONG_DEC,
    ELEV_IN_M,
    ELEV_IN_FT,
    MAP_NAME,
    DATE_CREATED,
    DATE_EDITED
};

struct Record {
    int offset{};
    std::string row;
    std::vector<std::string> rowVector;

    Record() = default;

    Record(int offset, std::string row) : offset(offset), row(std::move(row)) {
        rowVector = split();
    }

    // split string by delimiter
    std::vector<std::string> split() {
        std::string temp;
        for (char i: row) {
            if (i == '|') {
                rowVector.push_back(temp);
                temp = "";
            } else {
                temp += i;
            }
        }
        return rowVector;
    }

    int getKey() const {
        return offset;
    }

    std::string getValue() const {
        return row;
    }

    std::vector<std::string> getRowVector() const {
        return rowVector;
    }

    // str() function
    std::string str() const {
        // restore the row to original format
        std::string temp;
        for (const auto &i: rowVector) {
            temp += i + "|";
        }
        temp.pop_back();
        return temp;
    }

};

class RecordHash {
public:
    RecordHash() = default;

    // hash function
    size_t operator()(const Record &record) const {
        size_t hashVal = 0;
        auto rowVector = record.getRowVector();

        if (rowVector.empty()) {
            throw std::invalid_argument("Record is empty");
        }

        std::string d = rowVector[FEATURE_NAME] + rowVector[STATE_ALPHA];

        // The number 37 is a commonly used prime number in hash functions
        for (char i: d) {
            hashVal = 37 * hashVal + i;
        }

        return hashVal;
    }
};

class GISRecord {
public:
    GISRecord() {
        coordinateIndex = new CoordinateIndex();
        nameIndex = new NameIndex();
    };

    ~GISRecord() {
        delete coordinateIndex;
        delete nameIndex;
        coordinateIndex = new CoordinateIndex();
        nameIndex = new NameIndex();
    }

    void setBounds(float minLat, float maxLat, float minLong, float maxLong);

    void insertRecord(std::vector<std::string> row, int lineNum, int offset);

    std::vector<int> findRecords(float lat, float lng);

    std::vector<int> findRecords(float longitude, float latitude, float halfWidth, float halfHeight);

    std::vector<int> findRecords(const std::string &name, const std::string &state);

    int getNodeCount() {
        return coordinateIndex->getTotalLocations();
    };

    PRQuadTree &getTree() {
        return coordinateIndex->getTree();
    }

    std::string getHashTableStr() {
        return nameIndex->str();
    }

    //getBuffer
    // return   the buffer
    BufferPool<Record> &getBuffer() {
        return buffer;
    }

    std::vector<Record> getRecords(const std::vector<int> &offsets);


    int getImportedNames();

    int getLongestProbe();

    int getAvgNameLength();

    std::string dbFileName;

    // getPool()
    BufferPool<Record> &getPool() {
        return buffer;
    }

private:
    BufferPool<Record> buffer = BufferPool<Record>(15);
    CoordinateIndex *coordinateIndex;
    NameIndex *nameIndex;
};


#endif //INC_8042_PROJECT_GISRECORD_H
