#include "PRQuadTree.h"

/// https://www.geeksforgeeks.org/quad-tree/ used this for help

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

vector<Location> PRQuadTree::retrieve(vector<Location> returnNodes, const Location &location) {
    int index = getIndex(location);
    if (index != -1 && nodes[0] != nullptr) {
        nodes[index]->retrieve(returnNodes, location);
    }
    returnNodes.insert(returnNodes.end(), locations.begin(), locations.end());
    return returnNodes;
}

bool PRQuadTree::insert(const Location &location) {
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

    if (locations.size() > K_VALUE && level < MAX_LEVELS) {
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

int PRQuadTree::getIndex(const Location &location) const {
    int index = -1;
    float centerX = boundingBox.centerPoint.getX();
    float centerY = boundingBox.centerPoint.getY();

    float x = location.coordinate.getX();
    float y = location.coordinate.getY();

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

    nodes[0] = std::unique_ptr<PRQuadTree>(
            new PRQuadTree(K_VALUE, level + 1,
                           BoundingBox(Coordinate(centerX - quarterWidth,
                                                  centerY + quarterHeight), quarterWidth,
                                       quarterHeight), "tl"));
    nodes[1] = std::unique_ptr<PRQuadTree>(
            new PRQuadTree(K_VALUE, level + 1,
                           BoundingBox(Coordinate(centerX + quarterWidth,
                                                  centerY + quarterHeight), quarterWidth,
                                       quarterHeight), "tr"));
    nodes[2] = std::unique_ptr<PRQuadTree>(
            new PRQuadTree(K_VALUE, level + 1,
                           BoundingBox(Coordinate(centerX - quarterWidth,
                                                  centerY - quarterHeight), quarterWidth,
                                       quarterHeight), "bl"));
    nodes[3] = std::unique_ptr<PRQuadTree>(
            new PRQuadTree(K_VALUE, level + 1,
                           BoundingBox(Coordinate(centerX + quarterWidth,
                                                  centerY - quarterHeight), quarterWidth,
                                       quarterHeight), "br"));
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
            // check each location in the node if it has the same coordinates
            for (const auto &location: locations) {
                float locationX = location.coordinate.getX();
                float locationY = location.coordinate.getY();
                float compareX = coord.getX();
                float compareY = coord.getY();
                if (nearlyEqual(locationX, compareX, 0.00001) && nearlyEqual(locationY, compareY, 0.00001)) {
                    returnNodes.push_back(location);
                }

            }
        }
    }
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


Location &PRQuadTree::find(Coordinate coord) {
    static Location emptyLocation;
    // If the coordinate is not in this quad tree's bounding box, return an empty Location.
    if (!isCoordInBox(coord)) {
        return emptyLocation;
    }

    // If this is a leaf node, iterate over the locations to find the matching one.
    if (isLeaf()) {
        for (auto &location: locations) {
            if (location.getCoordinate().latitude == coord.latitude &&
                location.getCoordinate().longitude == coord.longitude) {
                return location;
            }
        }
    } else {
        // If this is an internal node, recursively call find on the correct child node.
        Location temp;
        temp.coordinate = coord;
        int index = getIndex(temp);
        if (index == -1) {
            return emptyLocation;
        }
        if (nodes[index] != nullptr) {
            return nodes[index]->find(coord);
        } else {
            return emptyLocation;
        }
    }

    // If we reach here, the location was not found, return an empty Location.
    return emptyLocation;
}

