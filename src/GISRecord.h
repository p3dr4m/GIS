#ifndef INC_8042_PROJECT_GISRECORD_H
#define INC_8042_PROJECT_GISRECORD_H

#include <string>
#include "Logger.h"
#include "CoordinateIndex.h"


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


private:
    Logger logger;
    CoordinateIndex *coordinateIndex;
};


#endif //INC_8042_PROJECT_GISRECORD_H
