//
// Created by p3dr4 on 2023-07-08.
//

#ifndef INC_8042_PROJECT_BOUNDINGBOX_H
#define INC_8042_PROJECT_BOUNDINGBOX_H


#include "Coordinate.h"

struct BoundingBox {
    Coordinate centerPoint;
    float halfWidth;
    float halfHeight;

    BoundingBox() {
        centerPoint = Coordinate(0, 0);
        halfWidth = 0;
        halfHeight = 0;
    }

    // Constructor to create bounding box search area
    BoundingBox(Coordinate center, float halfWidth, float halfHeight)
            : centerPoint(center), halfWidth(halfWidth), halfHeight(halfHeight) {}

    BoundingBox(float minLat, float maxLat, float minLong, float maxLong)
            : centerPoint((minLong + maxLong) / 2, (minLat + maxLat) / 2),
              halfWidth((maxLong - minLong) / 2),
              halfHeight((maxLat - minLat) / 2) {}

    Coordinate getTopLeft() const {
        return {centerPoint.longitude - halfWidth, centerPoint.latitude + halfHeight};
    }

    Coordinate getBottomRight() const {
        return {centerPoint.longitude + halfWidth, centerPoint.latitude - halfHeight};
    }
};

#endif //INC_8042_PROJECT_BOUNDINGBOX_H
