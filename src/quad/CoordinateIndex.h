#ifndef INC_8042_PROJECT_COORDINATEINDEX_H
#define INC_8042_PROJECT_COORDINATEINDEX_H

#include "PRQuadTree.h"
#include <vector>
#include <iostream>

class CoordinateIndex {
public:

    bool insert(float latitude, float longitude, int fileOffset, int dbLine);

    std::vector<int> searchRecords(float latitude, float longitude);

    std::vector<int> searchRecordsInBounds(Coordinate coord, float halfWidth, float halfHeight);

    void updateBoundsOfTree(BoundingBox boundingBox) {
        quadTree = PRQuadTree(0, boundingBox, "root");
    }

    int getTotalLocations() {
        return quadTree.getTotalLocations();
    }

    PRQuadTree &getTree() {
        // return reference to quadTree
        return quadTree;
    }

    void clear() {
        quadTree.clear();
    }


private:
    PRQuadTree quadTree = PRQuadTree(0, BoundingBox(), "root");

    static std::vector<int> getOffsetsFromNodes(std::vector<Location> nodes);
};


#endif //INC_8042_PROJECT_COORDINATEINDEX_H
