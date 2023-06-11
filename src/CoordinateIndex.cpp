#include "CoordinateIndex.h"

using namespace std;

std::vector<int> CoordinateIndex::searchRecords(float latitude, float longitude) {
    vector<Node> nodes;
    Coordinate coordinate = Coordinate(longitude, latitude);
    quadTree.getNodeByCoordinate(nodes, coordinate);

    // loop through the nodes and get the file offsets
    vector<int> fileOffsets = getOffsetsFromNodes(nodes);


    return fileOffsets;
}

std::vector<int> CoordinateIndex::searchRecordsInBounds(Coordinate coord, float halfWidth, float halfHeight) {
    vector<Node> nodes;
    BoundingBox boundingBox = BoundingBox(coord, halfWidth, halfHeight);
    quadTree.getNodesInBounds(nodes, boundingBox);

    // loop through the nodes and get the file offsets
    vector<int> fileOffsets = getOffsetsFromNodes(nodes);


    return fileOffsets;
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

std::vector<int> CoordinateIndex::getOffsetsFromNodes(std::vector<Node> nodes) {
    vector<int> fileOffsets;

    for (auto &node: nodes) {
        for (int index = 0; index < node.fileOffset.size(); index++) {
            fileOffsets.push_back(node.fileOffset[index]);
            fileOffsets.push_back(node.databaseLine[index]);
        }
    }

    return fileOffsets;
}
