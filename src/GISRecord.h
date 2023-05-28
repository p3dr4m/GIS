//
// Created by Pedram Nazari on 2023-04-28.
//

#ifndef INC_8042_PROJECT_GISRECORD_H
#define INC_8042_PROJECT_GISRECORD_H

#include <string>
#include "Logger.h"
using namespace std;



class GISRecord {
public:
    struct DMS {
        DMS() = default;

        int degrees;
        int minutes;
        int seconds;
        char direction;

        string toString() {
            return to_string(degrees) + "d " + to_string(minutes) + "m " +
                   to_string(seconds) + "s " + direction;
        }
    };



    float convertDMS(DMS dms);
    void setBounds(DMS minLat, DMS maxLat, DMS minLong, DMS maxLong);

    struct Bounds {
        float minLat;
        float maxLat;
        float minLong;
        float maxLong;
    } bounds;
private:
    Logger logger;
};


#endif //INC_8042_PROJECT_GISRECORD_H
