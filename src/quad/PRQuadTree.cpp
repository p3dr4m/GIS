#include <limits>
#include "PRQuadTree.h"

using namespace std;

bool PRQuadTree::isCoordInBox(Coordinate coord, BoundingBox box) {
    // Check if the coordinate is not within the latitude boundaries of the box
    if (coord.latitude < box.getBottomRight().latitude || coord.latitude > box.getTopLeft().latitude) {
        return false;
    }

    // Check if the coordinate is not within the longitude boundaries of the box
    if (coord.longitude < box.getTopLeft().longitude || coord.longitude > box.getBottomRight().longitude) {
        return false;
    }

    // If none of the above conditions are met, the coordinate is within the box
    return true;
}

vector<Location> PRQuadTree::retrieve(vector<Location> returnNodes, const Location &location) {
    int index = getIndex(location);
    if (index != -1 && nodes[0] != nullptr) {
        nodes[index]->retrieve(returnNodes, location);
    }
    returnNodes.insert(returnNodes.end(), locations.begin(), locations.end());
    return returnNodes;
}

bool PRQuadTree::insert(Location location) {
    // Check if the location is inside the bounding box of the quadtree
    if (!isCoordInBox(location.getCoordinate(), boundingBox)) {
        return false;  // Location is outside the bounding box, do nothing
    }


    // is it a leaf node?
    if (nodes[0] != nullptr) {
        // find the index of the node that the location belongs in
        int index = getIndex(location);
        if (index != -1) {
            nodes[index]->insert(location);
            return true;
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

    return true;
}

int PRQuadTree::getIndex(Location location) {
    int index = -1;
    float centerX = boundingBox.centerPoint.getX();
    float centerY = boundingBox.centerPoint.getY();

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
    float quarterWidth = boundingBox.halfWidth / 2;
    float quarterHeight = boundingBox.halfHeight / 2;
    float centerX = boundingBox.centerPoint.longitude;
    float centerY = boundingBox.centerPoint.latitude;

    nodes[0] = new PRQuadTree(level + 1,
                              BoundingBox(Coordinate(centerX - quarterWidth, centerY + quarterHeight), quarterWidth,
                                          quarterHeight), "tl");
    nodes[1] = new PRQuadTree(level + 1,
                              BoundingBox(Coordinate(centerX + quarterWidth, centerY + quarterHeight), quarterWidth,
                                          quarterHeight), "tr");
    nodes[2] = new PRQuadTree(level + 1,
                              BoundingBox(Coordinate(centerX - quarterWidth, centerY - quarterHeight), quarterWidth,
                                          quarterHeight), "bl");
    nodes[3] = new PRQuadTree(level + 1,
                              BoundingBox(Coordinate(centerX + quarterWidth, centerY - quarterHeight), quarterWidth,
                                          quarterHeight), "br");
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

bool nearlyEqual(float a, float b, float epsilon) {
    float absA = fabs(a);
    float absB = fabs(b);
    float diff = fabs(a - b);

    if (a == b) {
        return true;
    } else if (a == 0 || b == 0 || diff < numeric_limits<float>::min()) {
        return diff < epsilon;
    } else {
        return diff / (absA + absB) < epsilon;
    }
}

void PRQuadTree::getNodeByCoordinate(vector<Location> &returnNodes, Coordinate coord) {
    if (isCoordInBox(coord, boundingBox)) {
        if (nodes[0] != nullptr) {
            for (int i = 0; i < 4; i++) {
                nodes[i]->getNodeByCoordinate(returnNodes, coord);
            }
        } else {
            // check each location in the node if it has the same coordinates
            for (const auto &location: locations) {
                float locationX = location.getX();
                float locationY = location.getY();
                float compareX = coord.getX();
                float compareY = coord.getY();
                if (nearlyEqual(locationX, compareX, 0.00001) && nearlyEqual(locationY, compareY, 0.00001)) {
                    returnNodes.push_back(location);
                }

            }
        }
    }
}

bool isBoxIntersect(BoundingBox box1, BoundingBox box2) {
    // Check for intersection of two bounding boxes
    if (box1.getTopLeft().latitude < box2.getBottomRight().latitude ||
        box2.getTopLeft().latitude < box1.getBottomRight().latitude) {
        return false;
    }

    if (box1.getBottomRight().longitude < box2.getTopLeft().longitude ||
        box2.getBottomRight().longitude < box1.getTopLeft().longitude) {
        return false;
    }

    // If none of the above conditions are met, the boxes intersect
    return true;
}

void PRQuadTree::getLocationsInBounds(vector<Location> &returnNodes, BoundingBox box) {
    // Check if the bounding box of the current node intersects with the given box
    if (isBoxIntersect(boundingBox, box)) {
        if (nodes[0] != nullptr) {
            for (int i = 0; i < 4; i++) {
                nodes[i]->getLocationsInBounds(returnNodes, box);
            }
        } else {
            // Check each location in the node if it is inside the given box
            for (const auto &location: locations) {
                if (isCoordInBox(location.getCoordinate(), box)) {
                    returnNodes.push_back(location);
                }
            }
        }
    }
}


bool PRQuadTree::isLeaf() {
    return nodes[0] == nullptr;
}

int PRQuadTree::getLocationsSize() {
    return (int) locations.size();
}


vector<PRQuadTree *> PRQuadTree::getNodes() {
    return nodes;
}

vector<Location> PRQuadTree::getLocations() {
    return locations;
}

bool PRQuadTree::isCoordInBox(Coordinate coord) {
    // Check if the coordinate is not within the latitude boundaries of the box
    if (coord.latitude < boundingBox.getBottomRight().latitude || coord.latitude > boundingBox.getTopLeft().latitude) {
        return false;
    }

    // Check if the coordinate is not within the longitude boundaries of the box
    if (coord.longitude < boundingBox.getTopLeft().longitude ||
        coord.longitude > boundingBox.getBottomRight().longitude) {
        return false;
    }

    // If none of the above conditions are met, the coordinate is within the box
    return true;
}

