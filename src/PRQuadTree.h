#ifndef INC_8042_PROJECT_PRQUADTREE_H
#define INC_8042_PROJECT_PRQUADTREE_H


#include <string>
#include <memory>
#include <vector>


struct Coordinate {
    float longitude;
    float latitude;

    float getX() const {
        return longitude;
    }

    float getY() const {
        return latitude;
    }

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
    std::vector<int> fileOffset;
    std::vector<int> databaseLine;

    float getX() const {
        return coordinate.longitude;
    }

    float getY() const {
        return coordinate.latitude;
    }
};

struct BoundingBox {
    Coordinate centerPoint;
    Coordinate halfWidthHeights; // the half width and height of the box
    Coordinate topLeft;
    Coordinate bottomRight;

    //getCenter()
    Coordinate getCenter() {
        return centerPoint;
    }

    float getCenterX() {
        return centerPoint.longitude;
    }

    float getCenterY() {
        return centerPoint.latitude;
    }

    // Default constructor
    BoundingBox() {
        centerPoint = Coordinate(0, 0);
        halfWidthHeights = Coordinate(0, 0);
        topLeft = Coordinate(0, 0);
        bottomRight = Coordinate(0, 0);
    }

    BoundingBox(float minLat, float maxLat, float minLong, float maxLong);

    // Constructor to create bounding box search area
    BoundingBox(Coordinate coord, float halfWidth, float halfHeight);
};

// A QuadTree, containing 4 subtrees
class PRQuadTree {
private:
    int MAX_NODES = 4; // max number of nodes in a leaf node
    int MAX_LEVELS = 1000; // max number of levels in the tree
    int level; // the current level of the tree
    std::vector<Node> locations; // the locations in the tree
    BoundingBox boundingBox; // the bounding box of the tree
    std::vector<PRQuadTree *> nodes; // the subtrees of the tree
public:
    PRQuadTree(int pLevel, BoundingBox pBox) : level(pLevel), boundingBox(pBox) {
        nodes.reserve(4);
        // initialize the nodes to nullptr
        for (int i = 0; i < 4; i++) {
            nodes.push_back(nullptr);
        }
    };

    bool isLeaf();

    void clear();

    void split();

    int getIndex(Node location);;

    void insert(Node location);

    std::vector<Node> retrieve(std::vector<Node> returnNodes, const Node &location);

    bool isCoordInBox(Coordinate coord, BoundingBox box);

    bool isBoxInBox(BoundingBox box);

    int getTotalLocations();

    int getLocations();

    Coordinate getPoint();

    // getChildren returns the children of the node
    std::vector<PRQuadTree *> getChildren();

    //getBoundary()
    BoundingBox getBoundary() {
        return boundingBox;
    }

    void getLocationsInBounds(std::vector<Node> &returnNodes, BoundingBox box);

    void getNodeByCoordinate(std::vector<Node> &returnNodes, Coordinate coord);


};


#endif //INC_8042_PROJECT_PRQUADTREE_H
