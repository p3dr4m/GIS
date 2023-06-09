#include "CoordinateIndex.h"

using namespace std;

std::vector<int> CoordinateIndex::searchRecords(Coordinate centralLocation, float height, float width) {
    return {};
}

std::vector<int> CoordinateIndex::searchRecords(float latitude, float longitude) {
    return {};
}

void CoordinateIndex::insert(float latitude, float longitude, int fileOffset, int dbLine) {

    // create the Quad Tree Node
    Node node;
    node.coordinate.latitude = latitude;
    node.coordinate.longitude = longitude;
    node.fileOffset.push_back(fileOffset);
    node.databaseLine.push_back(dbLine);

    // insert the node into the Quad Tree in the coordinate index
    quadTree.insert(node);
}
