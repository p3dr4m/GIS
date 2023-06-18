#ifndef INC_8042_PROJECT_PRQUADTREE_H
#define INC_8042_PROJECT_PRQUADTREE_H


#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <cmath>


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

struct Location {
    Coordinate coordinate;
    std::vector<int> fileOffset;
    std::vector<int> databaseLine;

    Coordinate getCoordinate() const {
        return coordinate;
    }

    float getX() const {
        return coordinate.longitude;
    }

    float getY() const {
        return coordinate.latitude;
    }

    int getXInSec() const {
        return static_cast<int>(std::round(coordinate.longitude * 3600));
    }

    int getYInSec() const {
        return static_cast<int>(std::round(coordinate.latitude * 3600));
    }

    std::vector<int> getFileOffset() const {
        return fileOffset;
    }

    std::vector<int> getDatabaseLine() const {
        return databaseLine;
    }

};

struct BoundingBox {
    Coordinate centerPoint;
    Coordinate halfWidthHeights; // the half width and height of the box
    Coordinate topLeft;
    Coordinate bottomRight;

    float getCenterX() const {
        return centerPoint.longitude;
    }

    float getCenterY() const {
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
    int level = 0; // the current level of the tree
    std::vector<Location> locations; // the locations in the tree
    BoundingBox boundingBox; // the bounding box of the tree
    std::vector<PRQuadTree *> nodes; // the subtrees of the tree
    std::string id; // the id of the tree
public:
    PRQuadTree(int pLevel, BoundingBox pBox, std::string _id) : level(pLevel), boundingBox(pBox), id(std::move(_id)) {
        nodes.reserve(4);
        level = pLevel;
        // initialize the nodes to nullptr
        for (int i = 0; i < 4; i++) {
            nodes.push_back(nullptr);
        }
    };

    bool isLeaf();

    void clear();

    void split();

    int getIndex(Location location);;

    void insert(Location location);

    std::vector<Location> retrieve(std::vector<Location> returnNodes, const Location &location);

    bool isCoordInBox(Coordinate coord, BoundingBox box);

    bool isBoxInBox(BoundingBox box);

    int getTotalLocations();

    int getLocationsSize();
    std::vector<Location> getLocations();

    Coordinate getPoint();

    // getNodes returns the children of the node
    std::vector<PRQuadTree *> getNodes();

    int getLevel() const {
        return level;
    }

    BoundingBox getBoundary() {
        return boundingBox;
    }

    std::string getId() {
        return id;
    }

    void getLocationsInBounds(std::vector<Location> &returnNodes, BoundingBox box);

    void getNodeByCoordinate(std::vector<Location> &returnNodes, Coordinate coord);


};


#endif //INC_8042_PROJECT_PRQUADTREE_H
