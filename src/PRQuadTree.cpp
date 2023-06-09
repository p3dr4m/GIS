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
    if (!checkIfInBounds(node.coordinate, boundingBox)) {
        return;
    }

    // Check if current node has reached maximum nodes
    if (quadNodes.size() < maxNodes) {
        this->quadNodes.push_back(node);

    }
    else {
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

bool PRQuadTree::checkIfInBounds(Coordinate coord, BoundingBox box) {
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
