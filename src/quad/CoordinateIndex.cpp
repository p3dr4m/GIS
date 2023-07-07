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
    // Create the coordinate
    Coordinate coord = Coordinate(longitude, latitude);

    // Try to find the Location in the QuadTree
    Location &existingLocation = quadTree.find(coord);

    // Check if the Location already exists in the QuadTree
    if (existingLocation.coordinate.latitude == latitude && existingLocation.coordinate.longitude == longitude) {
        // If it exists, just update the fileOffset and databaseLine
        existingLocation.fileOffset.push_back(fileOffset);
        existingLocation.databaseLine.push_back(dbLine);
    } else {
        // If it doesn't exist, create a new Location and insert it into the QuadTree
        Location newNode;
        newNode.coordinate = coord;
        newNode.fileOffset.push_back(fileOffset);
        newNode.databaseLine.push_back(dbLine);

        return quadTree.insert(newNode);
    }

    return true;
}

vector<int> CoordinateIndex::getOffsetsFromNodes(vector<Location> nodes) {
    vector<int> fileOffsets;

    for (auto &node: nodes) {
        for (int index = 0; index < node.fileOffset.size(); index++) {
            fileOffsets.push_back(node.fileOffset[index]);
        }
    }

    return fileOffsets;
}

