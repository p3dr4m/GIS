#include "PRQuadTree.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace std;

void PRQuadTree::setBoundary(Bounds bounds) {
    float boxHalfWidthLat = (bounds.maxLat - bounds.minLat) / 2;
    float boxHalfWidthLong = (bounds.maxLong - bounds.minLong) / 2;
    float centerLong = (bounds.minLong + bounds.maxLong) / 2;
    float centerLat = (bounds.minLat + bounds.maxLat) / 2;

    boundingBox.topLeft = Coordinate(bounds.minLong, bounds.maxLat);
    boundingBox.bottomRight = Coordinate(bounds.maxLong, bounds.minLat);
    boundingBox.centerPoint = Coordinate(centerLong, centerLat);
    boundingBox.boxHalfWidth = Coordinate(boxHalfWidthLong, boxHalfWidthLat);

}

void PRQuadTree::insert(Node node) {
    if (!isCoordInBox(node.coordinate, boundingBox)) {
        return;
    }

    // Check if current node has reached maximum nodes
    if (quadNodes.size() < maxNodes) {
        this->quadNodes.push_back(node);

    } else {
        // Subdivide the tree and insert the node into the appropriate quadrant
        if (topLeftQuad == nullptr) {
            // Split the bounding box into 4 quadrants
            Bounds topLeftBounds = {boundingBox.centerPoint.latitude, boundingBox.topLeft.latitude,
                                    boundingBox.topLeft.longitude, boundingBox.centerPoint.longitude};
            topLeftQuad = new PRQuadTree();
            topLeftQuad->setBoundary(topLeftBounds);

            Bounds topRightBounds = {boundingBox.centerPoint.latitude, boundingBox.topLeft.latitude,
                                     boundingBox.centerPoint.longitude, boundingBox.bottomRight.longitude};
            topRightQuad = new PRQuadTree();
            topRightQuad->setBoundary(topRightBounds);

            Bounds bottomLeftBounds = {boundingBox.bottomRight.latitude, boundingBox.centerPoint.latitude,
                                       boundingBox.topLeft.longitude, boundingBox.centerPoint.longitude};
            bottomLeftQuad = new PRQuadTree();
            bottomLeftQuad->setBoundary(bottomLeftBounds);

            Bounds bottomRightBounds = {boundingBox.bottomRight.latitude, boundingBox.centerPoint.latitude,
                                        boundingBox.centerPoint.longitude, boundingBox.bottomRight.longitude};
            bottomRightQuad = new PRQuadTree();
            bottomRightQuad->setBoundary(bottomRightBounds);
        }

        // Attempt to insert node into each quadrant
        topLeftQuad->insert(node);
        topRightQuad->insert(node);
        bottomLeftQuad->insert(node);
        bottomRightQuad->insert(node);
    }

}

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

bool PRQuadTree::isBoxinBox(BoundingBox box) {
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


int PRQuadTree::countAllQuadNodes() {
    int count = 0;
    count += quadNodes.size();

    if (topLeftQuad != nullptr) {
        count += topLeftQuad->countAllQuadNodes();
    }
    if (topRightQuad != nullptr) {
        count += topRightQuad->countAllQuadNodes();
    }
    if (bottomLeftQuad != nullptr) {
        count += bottomLeftQuad->countAllQuadNodes();
    }
    if (bottomRightQuad != nullptr) {
        count += bottomRightQuad->countAllQuadNodes();
    }

    return count;
}

void PRQuadTree::getNodesInBounds(std::vector<Node> &nodes, BoundingBox box) {
    // Check if the current node is within the bounds
    for (auto &quadNode: quadNodes) {
        if (isCoordInBox(quadNode.coordinate, box)) {
            nodes.push_back(quadNode);
        }
    }

    // check if quadtree has been subdivided
    if (topLeftQuad != nullptr) {
        // Check if the top left quadrant is within the bounds
        if (isBoxinBox(topLeftQuad->boundingBox)) {
            topLeftQuad->getNodesInBounds(nodes, box);
        }

        // Check if the top right quadrant is within the bounds
        if (isBoxinBox(topRightQuad->boundingBox)) {
            topRightQuad->getNodesInBounds(nodes, box);
        }

        // Check if the bottom left quadrant is within the bounds
        if (isBoxinBox(bottomLeftQuad->boundingBox)) {
            bottomLeftQuad->getNodesInBounds(nodes, box);
        }

        // Check if the bottom right quadrant is within the bounds
        if (isBoxinBox(bottomRightQuad->boundingBox)) {
            bottomRightQuad->getNodesInBounds(nodes, box);
        }
    }

}

void PRQuadTree::getNodeByCoordinate(vector<Node> &nodes, Coordinate coord) {

    if (!isCoordInBox(coord, this->boundingBox)) {
        return;
    }

    for (auto &quadNode: quadNodes) {
        if (quadNode.coordinate.latitude == coord.latitude && quadNode.coordinate.longitude == coord.longitude) {
            nodes.push_back(quadNode);
        }
    }

    // Check if the coordinate is in the top left quadrant
    if (coord.longitude <= boundingBox.boxHalfWidth.longitude) {
        if (topLeftQuad != nullptr) {
            topLeftQuad->getNodeByCoordinate(nodes, coord);

        }
    }

    // Check if the coordinate is in the top right quadrant
    if (coord.latitude >= boundingBox.boxHalfWidth.latitude) {
        if (topRightQuad != nullptr) {
            topRightQuad->getNodeByCoordinate(nodes, coord);
        }
    }


    // Check if the coordinate is in the bottom left quadrant
    if (coord.latitude <= boundingBox.boxHalfWidth.latitude) {
        if (bottomLeftQuad != nullptr) {
            bottomLeftQuad->getNodeByCoordinate(nodes, coord);
        }
    }

    // Check if the coordinate is in the bottom right quadrant
    if (coord.longitude >= boundingBox.boxHalfWidth.longitude) {
        if (bottomRightQuad != nullptr) {
            bottomRightQuad->getNodeByCoordinate(nodes, coord);
        }
    }
}

BoundingBox::BoundingBox(Coordinate centerPoint, float halfWidth, float halfHeight)
        : centerPoint(centerPoint),
          boxHalfWidth(halfWidth, halfHeight),
          topLeft(centerPoint.longitude - halfWidth, centerPoint.latitude + halfHeight) {
    this->centerPoint = centerPoint;
    this->boxHalfWidth = Coordinate(halfWidth, halfHeight);
    this->topLeft = Coordinate(centerPoint.longitude - halfWidth, centerPoint.latitude + halfHeight);
    this->bottomRight = Coordinate(centerPoint.longitude + halfWidth, centerPoint.latitude - halfHeight);
}
