#ifndef INC_8042_PROJECT_GISRECORD_H
#define INC_8042_PROJECT_GISRECORD_H

#include <string>
#include "CoordinateIndex.h"
#include "NameIndex.h"
#include <stdexcept>


struct DMS {
    int degrees;
    int minutes;
    int seconds;
    char direction;

    DMS() = default;


    explicit DMS(const std::string &dms);

    std::string toString() const;

    std::string toTotalSeconds() const;

    float toFloat() const;
};

enum GIS_Record_Header {
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

    void insertRecord(std::vector<std::string> row, int lineNum, int offset);

    std::vector<int> findRecords(float lat, float lng);

    std::vector<int> findRecords(float longitude, float latitude, float halfWidth, float halfHeight);

    int getNodeCount() {
        return coordinateIndex->getNodeCount();
    };

    int getImportedNames();

    int getLongestProbe();

    int getAvgNameLength();

private:
    CoordinateIndex *coordinateIndex;
    NameIndex *nameIndex;

};


#endif //INC_8042_PROJECT_GISRECORD_H
