//
// Created by Raziq on 2023-04-26.
//

#ifndef FINALPROJALGO_COORDINATEINDEX_H
#define FINALPROJALGO_COORDINATEINDEX_H
#include "QuadTree.h"


class CoordinateIndex {
public:
    void insertRecord(float latitude, float longitude, int fileOffset,
                      int dbLine);
    std::vector<int> searchRecords(float latitude, float longitude);
    std::vector<int> searchRecords(Coordinate centralLocation, float height,
                                   float width);

    void updateBoundsOfTree(float minLat, float minLong, float maxLat,
                            float maxLong) {
        m_tree.setBoundry(minLat, minLong, maxLat, maxLong);
    }

    std::string printTree() { return m_tree.toString(1); }

private:
    QuadTree m_tree;
};


#endif //FINALPROJALGO_COORDINATEINDEX_H
