#ifndef INC_8042_PROJECT_COORDINATEINDEX_H
#define INC_8042_PROJECT_COORDINATEINDEX_H

#include "PRQuadTree.h"
#include "../structs/BoundingBox.h"

class CoordinateIndex {
public:

    bool insert(float latitude, float longitude, int fileOffset, int dbLine);

    std::vector<int> searchRecords(float latitude, float longitude);

    std::vector<int> searchRecordsInBounds(Coordinate coord, float halfWidth, float halfHeight);

    void updateBoundsOfTree(BoundingBox boundingBox) {
        quadTree = PRQuadTree(4, 0, boundingBox, "root");
    }

    PRQuadTree &getTree() {
        return quadTree;
    }

    void clear() {
        quadTree.clear();
    }


private:
    PRQuadTree quadTree;

    static std::vector<int> getOffsetsFromNodes(const std::vector<Location> &nodes);
};


#endif //INC_8042_PROJECT_COORDINATEINDEX_H
