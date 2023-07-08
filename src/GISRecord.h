#ifndef INC_8042_PROJECT_GISRECORD_H
#define INC_8042_PROJECT_GISRECORD_H

#include "quad/CoordinateIndex.h"
#include "hash/NameIndex.h"
#include "BufferPool.h"
#include "structs/Record.h"

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

    void
    insertRecord(int offset, std::vector<std::string> row, int lineNum, int &importedRecords, int &importedLocations);

    std::vector<int> findRecords(float lat, float lng);

    std::vector<int> findRecords(float longitude, float latitude, float halfWidth, float halfHeight);

    std::vector<int> findRecords(const std::string &name, const std::string &state);

    PRQuadTree &getTree() {
        return coordinateIndex->getTree();
    }

    std::string getHashTableStr() {
        return nameIndex->str();
    }

    std::vector<Record> getRecords(const std::vector<int> &offsets);

    std::vector<Record> getRecords(const std::vector<int> &offsets, const std::string &filterOption);

    int getLongestProbe() {
        return nameIndex->getLongestProbe();
    }


    BufferPool<Record> &getBuffer() {
        return buffer;
    }

    int getHashCapacity() {
        return nameIndex->getCapacity();
    }

    int getHashSize() {
        return nameIndex->getSize();
    }

    void clearIndices() {
        coordinateIndex->clear();
        nameIndex->clear();
    }


    std::string dbFileName;
private:
    BufferPool<Record> buffer = BufferPool<Record>(15);
    CoordinateIndex *coordinateIndex;
    NameIndex *nameIndex;
};


#endif //INC_8042_PROJECT_GISRECORD_H
