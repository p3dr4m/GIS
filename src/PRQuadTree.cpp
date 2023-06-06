#include "PRQuadTree.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace std;

void PRQuadTree::setBoundary(Bounds bounds) {
    float halfWidth = (bounds.maxLat - bounds.minLat) / 2;
    float halfHeight = (bounds.maxLong - bounds.minLong) / 2;

    boundingBox.topLeft = Coordinate(bounds.minLong, bounds.maxLat);
    boundingBox.bottomRight = Coordinate(bounds.maxLong, bounds.minLat);
    boundingBox.centerPoint = Coordinate((bounds.minLong + bounds.maxLong) / 2, halfHeight);
    boundingBox.boxHalfWidth = Coordinate((bounds.maxLong - bounds.minLong) / 2, halfWidth);

}

void PRQuadTree::insert(Node node) {
    if (!checkIfInBounds(node.coordinate, boundingBox)) {
        return;
    }

    // Check if current node has reached maximum nodes
    if (quadNodes.size() < maxNodes)
        quadNodes.push_back(node);
    else {
        // Subdivide the tree and insert the node into the appropriate quadrant
        if (topLeftQuad == nullptr) {
            // Split the bounding box into 4 quadrants
            Bounds topLeftBounds = {boundingBox.topLeft.latitude, boundingBox.centerPoint.latitude,
                                    boundingBox.topLeft.longitude, boundingBox.centerPoint.longitude};
            topLeftQuad = new PRQuadTree();
            topLeftQuad->setBoundary(topLeftBounds);

            Bounds topRightBounds = {boundingBox.topLeft.latitude, boundingBox.centerPoint.latitude,
                                     boundingBox.centerPoint.longitude, boundingBox.bottomRight.longitude};
            topRightQuad = new PRQuadTree();
            topRightQuad->setBoundary(topRightBounds);

            Bounds bottomLeftBounds = {boundingBox.centerPoint.latitude, boundingBox.bottomRight.latitude,
                                       boundingBox.topLeft.longitude, boundingBox.centerPoint.longitude};
            bottomLeftQuad = new PRQuadTree();
            bottomLeftQuad->setBoundary(bottomLeftBounds);

            Bounds bottomRightBounds = {boundingBox.centerPoint.latitude, boundingBox.bottomRight.latitude,
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
    return coord.latitude >= box.bottomRight.latitude && coord.latitude <= box.topLeft.latitude &&
           coord.longitude >= box.topLeft.longitude && coord.longitude <= box.bottomRight.longitude;
}

