#ifndef INC_8042_PROJECT_GISRECORD_H
#define INC_8042_PROJECT_GISRECORD_H

#include <string>
#include "Logger.h"
#include "CoordinateIndex.h"
#include "NameIndex.h"


struct DMS {
    int degrees;
    int minutes;
    int seconds;
    char direction;

    DMS() = default;

    explicit DMS(std::string dms) {
        degrees = std::stoi(dms.substr(0, dms.find('d')));
        minutes = std::stoi(dms.substr(dms.find('d') + 1, dms.find('m')));
        seconds = std::stoi(dms.substr(dms.find('m') + 1, dms.find('s')));
        direction = dms[dms.length() - 1];
    }

    std::string toString() const {
        return std::to_string(degrees) + "d " + std::to_string(minutes) + "m " +
               std::to_string(seconds) + "s " + direction;
    }

    float toFloat() const {
        auto decimalVersion = static_cast<float>(degrees);
        decimalVersion += static_cast<float>(minutes) / 60.f;
        decimalVersion += static_cast<float>(seconds) / 3600.f;
        return decimalVersion;
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
        nameIndex = new NameIndex();
    };

    ~GISRecord() {
        delete coordinateIndex;
        delete nameIndex;
        coordinateIndex = new CoordinateIndex();
        nameIndex = new NameIndex();
    }

    Bounds bounds{};

    float convertDmsToFloat(DMS dms);

    void setBounds(float minLat, float maxLat, float minLong, float maxLong);

    void insertRecord(std::vector<std::string> row, int lineNum, int offset);

    std::vector<int> findRecords(float lat, float lng);

    std::vector<int> findRecords(float longitude, float latitude, float halfWidth, float halfHeight);

    void printNodes() {
        coordinateIndex->printNodeCountOfTree();
    };
private:
    Logger logger;
    CoordinateIndex *coordinateIndex;
    NameIndex *nameIndex;

};


#endif //INC_8042_PROJECT_GISRECORD_H
