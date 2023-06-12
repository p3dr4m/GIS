#ifndef INC_8042_PROJECT_COORDINATEINDEX_H
#define INC_8042_PROJECT_COORDINATEINDEX_H

#include "PRQuadTree.h"
#include <vector>
#include <iostream>

class CoordinateIndex {
public:
    void insert(float latitude, float longitude, int fileOffset, int dbLine);

    std::vector<int> searchRecords(float latitude, float longitude);

    std::vector<int> searchRecordsInBounds(Coordinate coord, float halfWidth, float halfHeight);

    void updateBoundsOfTree(Bounds bounds) {
        quadTree.setBoundary(bounds);
    }

    int getNodeCount() {
        return quadTree.countAllQuadNodes();
    }


private:
    PRQuadTree quadTree;
    static std::vector<int> getOffsetsFromNodes(std::vector<Node> nodes);
};


#endif //INC_8042_PROJECT_COORDINATEINDEX_H
