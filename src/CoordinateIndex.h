#ifndef INC_8042_PROJECT_COORDINATEINDEX_H
#define INC_8042_PROJECT_COORDINATEINDEX_H

#include "PRQuadTree.h"
#include <vector>


class CoordinateIndex {
public:
    void insertRecord(float latitude, float longitude, int fileOffset,
                      int dbLine);

    std::vector<int> searchRecords(float latitude, float longitude);

    std::vector<int> searchRecords(Coordinate centralLocation, float height,
                                   float width);

    void updateBoundsOfTree(Bounds bounds) {
        quadTree.setBoundary(bounds);
    }


private:
    PRQuadTree quadTree;
};


#endif //INC_8042_PROJECT_COORDINATEINDEX_H
