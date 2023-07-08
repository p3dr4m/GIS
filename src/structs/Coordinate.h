#ifndef INC_8042_PROJECT_COORDINATE_H
#define INC_8042_PROJECT_COORDINATE_H

struct Coordinate {
    float longitude;
    float latitude;

    float getX() const {
        return longitude;
    }

    float getY() const {
        return latitude;
    }

    Coordinate(float x, float y) {
        longitude = x;
        latitude = y;
    }

    Coordinate() {
        longitude = 0;
        latitude = 0;
    }
};

#endif //INC_8042_PROJECT_COORDINATE_H
