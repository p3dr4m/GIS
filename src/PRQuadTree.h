#ifndef INC_8042_PROJECT_PRQUADTREE_H
#define INC_8042_PROJECT_PRQUADTREE_H


#include <string>
#include <memory>
#include <vector>

struct Bounds {
    float minLat;
    float maxLat;
    float minLong;
    float maxLong;
};

struct Coordinate {
    float latitude;
    float longitude;

    Coordinate(float x, float y) {
        longitude = x;
        latitude = y;
    }

    Coordinate() {
        longitude = 0;
        latitude = 0;
    }
};

struct Node {
    Coordinate coordinate;
};

struct BoundingBox {
    Coordinate centerPoint;
    Coordinate boxHalfWidth;
    Coordinate topLeft;
    Coordinate bottomRight;

    BoundingBox() = default;
};

// A QuadTree, containing 4 subtrees
class PRQuadTree {
private:
    BoundingBox boundingBox;
    int maxNodes = 4;
    std::vector<Node> quadNodes;

    // Children of tree
    PRQuadTree *topLeftQuad;
    PRQuadTree *topRightQuad;
    PRQuadTree *bottomLeftQuad;
    PRQuadTree *bottomRightQuad;
public:
    PRQuadTree()
            : boundingBox{Coordinate(), Coordinate(), Coordinate(), Coordinate()},
              topLeftQuad(nullptr),
              topRightQuad(nullptr),
              bottomLeftQuad(nullptr),
              bottomRightQuad(nullptr) {
    }

    void setBoundary(Bounds bounds);

    void insert(Node node);

    bool checkIfInBounds(Coordinate coord, BoundingBox box);
};


#endif //INC_8042_PROJECT_PRQUADTREE_H
