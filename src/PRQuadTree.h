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
    float longitude;
    float latitude;

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
    std::vector<int> fileOffset, databaseLine;
};

struct BoundingBox {
    Coordinate centerPoint;
    Coordinate boxHalfWidth;
    Coordinate topLeft;
    Coordinate bottomRight;

    BoundingBox() = default;

    // Constructor to create bounding box search area
    BoundingBox(Coordinate centerPoint, float halfWidth, float halfHeight);
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
            : boundingBox{},
              topLeftQuad(nullptr),
              topRightQuad(nullptr),
              bottomLeftQuad(nullptr),
              bottomRightQuad(nullptr) {
    }

    void setBoundary(Bounds bounds);

    void insert(Node node);

    bool isCoordInBox(Coordinate coord, BoundingBox box);

    bool isBoxinBox(BoundingBox box);

    void getNodeByCoordinate(std::vector<Node> &nodes, Coordinate coord);

    void getNodesInBounds(std::vector<Node> &nodes, BoundingBox box);


    int countAllQuadNodes();

};


#endif //INC_8042_PROJECT_PRQUADTREE_H
