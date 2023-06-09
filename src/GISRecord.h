#ifndef INC_8042_PROJECT_GISRECORD_H
#define INC_8042_PROJECT_GISRECORD_H

#include <string>
#include "Logger.h"
#include "CoordinateIndex.h"
#include "NameIndex.h"


struct DMS {
    DMS() = default;

    int degrees;
    int minutes;
    int seconds;
    char direction;

    std::string toString() {
        return std::to_string(degrees) + "d " + std::to_string(minutes) + "m " +
               std::to_string(seconds) + "s " + direction;
    }
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
    };

    ~GISRecord() {
        delete coordinateIndex;
        coordinateIndex = new CoordinateIndex();
    }

    Bounds bounds{};

    float convertDMS(DMS dms);

    void setBounds(DMS minLat, DMS maxLat, DMS minLong, DMS maxLong);
    void insertRecord(std::vector<std::string> row, int lineNum, int offset);
    void printNodes() {
        coordinateIndex->printNodeCountOfTree();
    };
private:
    Logger logger;
    CoordinateIndex *coordinateIndex;
    NameIndex *nameIndex;
};


#endif //INC_8042_PROJECT_GISRECORD_H
