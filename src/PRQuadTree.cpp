#include "PRQuadTree.h"

using namespace std;

bool PRQuadTree::isCoordInBox(Coordinate coord, BoundingBox box) {
    // Check if the coordinate is not within the latitude boundaries of the box
    if (coord.latitude < box.bottomRight.latitude || coord.latitude > box.topLeft.latitude) {
        return false;
    }

    // Check if the coordinate is not within the longitude boundaries of the box
    if (coord.longitude < box.topLeft.longitude || coord.longitude > box.bottomRight.longitude) {
        return false;
    }

    // If none of the above conditions are met, the coordinate is within the box
    return true;
}

bool PRQuadTree::isBoxInBox(BoundingBox box) {
    // Check if the box is not within the latitude boundaries of the box
    if (box.bottomRight.latitude < boundingBox.bottomRight.latitude ||
        box.topLeft.latitude > boundingBox.topLeft.latitude) {
        return false;
    }

    // Check if the box is not within the longitude boundaries of the box
    if (box.topLeft.longitude < boundingBox.topLeft.longitude ||
        box.bottomRight.longitude > boundingBox.bottomRight.longitude) {
        return false;
    }

    // If none of the above conditions are met, the box is within the box
    return true;
}

std::vector<Location> PRQuadTree::retrieve(std::vector<Location> returnNodes, const Location &location) {
    int index = getIndex(location);
    if (index != -1 && nodes[0] != nullptr) {
        nodes[index]->retrieve(returnNodes, location);
    }
    returnNodes.insert(returnNodes.end(), locations.begin(), locations.end());
    return returnNodes;
}

void PRQuadTree::insert(Location location) {
    // Check if the location is inside the bounding box of the quadtree
    if (!isCoordInBox(location.getCoordinate(), boundingBox)) {
        return;  // Location is outside the bounding box, do nothing
    }


    // is it a leaf node?
    if (nodes[0] != nullptr) {
        // find the index of the node that the location belongs in
        int index = getIndex(location);
        if (index != -1) {
            nodes[index]->insert(location);
            return;
        }
    }

    locations.push_back(location);

    if (locations.size() > MAX_NODES && level < MAX_LEVELS) {
        if (nodes[0] == nullptr) {
            split();
        }
        for (int i = 0; i < locations.size();) {
            int index = getIndex(locations[i]);
            if (index != -1) {
                nodes[index]->insert(locations[i]);
                locations.erase(locations.begin() + i);
            } else {
                i++;
            }
        }
    }
}

int PRQuadTree::getIndex(Location location) {
    int index = -1;
    float centerX = boundingBox.getCenterX();
    float centerY = boundingBox.getCenterY();

    float x = location.getX();
    float y = location.getY();

    // Object can completely fit within the top quadrants
    bool topQuadrant = (y > centerY);
    // Object can completely fit within the bottom quadrants
    bool bottomQuadrant = (y < centerY);

    // Object can completely fit within the left quadrants
    if (x < centerX) {
        if (topQuadrant) {
            index = 0;
        } else if (bottomQuadrant) {
            index = 2;
        }
    }
        // Object can completely fit within the right quadrants
    else if (x > centerX) {
        if (topQuadrant) {
            index = 1;
        } else if (bottomQuadrant) {
            index = 3;
        }
    }
    return index;
}

void PRQuadTree::split() {
    float halfWidth = boundingBox.halfWidthHeights.longitude / 2;
    float halfHeight = boundingBox.halfWidthHeights.latitude / 2;
    float x = boundingBox.centerPoint.longitude;
    float y = boundingBox.centerPoint.latitude;

    nodes[0] = new PRQuadTree(level + 1,
                              BoundingBox(Coordinate(x - halfWidth, y + halfHeight), halfWidth, halfHeight));
    nodes[1] = new PRQuadTree(level + 1,
                              BoundingBox(Coordinate(x + halfWidth, y + halfHeight), halfWidth, halfHeight));
    nodes[2] = new PRQuadTree(level + 1,
                              BoundingBox(Coordinate(x - halfWidth, y - halfHeight), halfWidth, halfHeight));
    nodes[3] = new PRQuadTree(level + 1,
                              BoundingBox(Coordinate(x + halfWidth, y - halfHeight), halfWidth, halfHeight));
}

void PRQuadTree::clear() {
    locations.clear();
    for (auto &node: nodes) {
        if (node != nullptr) {
            node->clear();
            node = nullptr;
        }
    }
}

int PRQuadTree::getTotalLocations() {
    int totalLocations = (int) locations.size();
    for (auto &node: nodes) {
        if (node != nullptr) {
            totalLocations += node->getTotalLocations();
        }
    }
    return totalLocations;
}

void PRQuadTree::getNodeByCoordinate(vector<Location> &returnNodes, Coordinate coord) {
    if (isCoordInBox(coord, boundingBox)) {
        if (nodes[0] != nullptr) {
            for (int i = 0; i < 4; i++) {
                nodes[i]->getNodeByCoordinate(returnNodes, coord);
            }
        } else {
            returnNodes.insert(returnNodes.end(), locations.begin(), locations.end());
        }
    }
}

void PRQuadTree::getLocationsInBounds(vector<Location> &returnNodes, BoundingBox box) {
    // recursively get all locations that are in the box
    if (isBoxInBox(box)) {
        if (nodes[0] != nullptr) {
            for (auto node: nodes) {
                node->getLocationsInBounds(returnNodes, box);
            }
        } else {
            returnNodes.insert(returnNodes.end(), locations.begin(), locations.end());
        }
    } else {
        returnNodes.insert(returnNodes.end(), locations.begin(), locations.end());
    }
}

bool PRQuadTree::isLeaf() {
    return nodes[0] == nullptr;
}

int PRQuadTree::getLocationsSize() {
    return (int) locations.size();
}

Coordinate PRQuadTree::getPoint() {
    return boundingBox.centerPoint;
}

std::vector<PRQuadTree *> PRQuadTree::getNodes() {
    return nodes;
}

vector<Location> PRQuadTree::getLocations() {
    return locations;
}


BoundingBox::BoundingBox(Coordinate coord, float halfWidth, float halfHeight) {
    this->centerPoint = coord;
    this->halfWidthHeights = Coordinate(halfWidth, halfHeight);
    this->topLeft = Coordinate(coord.longitude - halfWidth, coord.latitude + halfHeight);
    this->bottomRight = Coordinate(coord.longitude + halfWidth, coord.latitude - halfHeight);
}

BoundingBox::BoundingBox(float minLat, float maxLat, float minLong, float maxLong) {
    this->centerPoint = Coordinate((minLong + maxLong) / 2, (minLat + maxLat) / 2);
    this->halfWidthHeights = Coordinate((maxLong - minLong) / 2, (maxLat - minLat) / 2);
    this->topLeft = Coordinate(minLong, maxLat);
    this->bottomRight = Coordinate(maxLong, minLat);
}

