#include "CoordinateIndex.h"

using namespace std;

vector<int> CoordinateIndex::searchRecords(float latitude, float longitude) {
    vector<Location> nodes;
    Coordinate coordinate = Coordinate(longitude, latitude);
    quadTree.getNodeByCoordinate(nodes, coordinate);
    // loop through the nodes and get the file offsets
    vector<int> fileOffsets = getOffsetsFromNodes(nodes);


    return fileOffsets;
}

vector<int> CoordinateIndex::searchRecordsInBounds(Coordinate coord, float halfWidth, float halfHeight) {
    vector<Location> nodes;
    BoundingBox boundingBox = BoundingBox(coord, halfWidth, halfHeight);
    quadTree.getLocationsInBounds(nodes, boundingBox);

    // loop through the nodes and get the file offsets
    vector<int> fileOffsets = getOffsetsFromNodes(nodes);


    return fileOffsets;
}

bool CoordinateIndex::insert(float latitude, float longitude, int fileOffset, int dbLine) {

    // create the Quad Tree Location
    Location node;
    node.coordinate.latitude = latitude;
    node.coordinate.longitude = longitude;
    node.fileOffset.push_back(fileOffset);
    node.databaseLine.push_back(dbLine);

    // insert the node into the Quad Tree in the coordinate index
    return quadTree.insert(node);
}

vector<int> CoordinateIndex::getOffsetsFromNodes(vector<Location> nodes) {
    vector<int> fileOffsets;

    for (auto &node: nodes) {
        for (int index = 0; index < node.fileOffset.size(); index++) {
            fileOffsets.push_back(node.fileOffset[index]);
//            fileOffsets.push_back(node.databaseLine[index]);
        }
    }

    return fileOffsets;
}

