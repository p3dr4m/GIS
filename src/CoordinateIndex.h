#ifndef INC_8042_PROJECT_COORDINATEINDEX_H
#define INC_8042_PROJECT_COORDINATEINDEX_H

#include "PRQuadTree.h"


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

    std::string printTree() { return quadTree.toString(1); }

private:
    PRQuadTree quadTree;
};


#endif //INC_8042_PROJECT_COORDINATEINDEX_H
