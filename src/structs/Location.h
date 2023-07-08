//
// Created by p3dr4 on 2023-07-08.
//

#ifndef INC_8042_PROJECT_LOCATION_H
#define INC_8042_PROJECT_LOCATION_H

#include "Coordinate.h"
#include <cmath>
#include <vector>

struct Location {
    Coordinate coordinate;
    std::vector<int> fileOffset;
    std::vector<int> databaseLine;

    Coordinate getCoordinate() const {
        return coordinate;
    }

    int getXInSec() const {
        return static_cast<int>(std::round(coordinate.longitude * 3600));
    }

    int getYInSec() const {
        return static_cast<int>(std::round(coordinate.latitude * 3600));
    }

};

#endif //INC_8042_PROJECT_LOCATION_H
