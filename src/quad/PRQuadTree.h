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
    float halfWidth;
    float halfHeight;

    BoundingBox() {
        centerPoint = Coordinate(0, 0);
        halfWidth = 0;
        halfHeight = 0;
    }

    // Constructor to create bounding box search area
    BoundingBox(Coordinate center, float halfWidth, float halfHeight)
            : centerPoint(center), halfWidth(halfWidth), halfHeight(halfHeight) {}

    BoundingBox(float minLat, float maxLat, float minLong, float maxLong)
            : centerPoint((minLong + maxLong) / 2, (minLat + maxLat) / 2),
              halfWidth((maxLong - minLong) / 2),
              halfHeight((maxLat - minLat) / 2) {}

    Coordinate getTopLeft() const {
        return {centerPoint.longitude - halfWidth, centerPoint.latitude + halfHeight};
    }

    Coordinate getBottomRight() const {
        return {centerPoint.longitude + halfWidth, centerPoint.latitude - halfHeight};
    }
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

    ~PRQuadTree() {
        for (int i = 0; i < 4; i++) {
            delete nodes[i];
        }
    }

    // copy constructor
    PRQuadTree(const PRQuadTree &other) {
        level = other.level;
        boundingBox = other.boundingBox;
        nodes = other.nodes;
        id = other.id;
    }

    // copy assignment operator
    PRQuadTree &operator=(const PRQuadTree &other) {
        level = other.level;
        boundingBox = other.boundingBox;
        nodes = other.nodes;
        id = other.id;
        return *this;
    }

    // move constructor
    PRQuadTree(PRQuadTree &&other) noexcept {
        level = other.level;
        boundingBox = other.boundingBox;
        nodes = other.nodes;
        id = other.id;
    }

    // move assignment operator
    PRQuadTree &operator=(PRQuadTree &&other) noexcept {
        level = other.level;
        boundingBox = other.boundingBox;
        nodes = other.nodes;
        id = other.id;
        return *this;
    }

    bool isLeaf();

    void clear();

    void split();

    int getIndex(Location location);;

    bool insert(Location location);

    std::vector<Location> retrieve(std::vector<Location> returnNodes, const Location &location);

    bool isCoordInBox(Coordinate coord, BoundingBox box);
    bool isCoordInBox(Coordinate coord);

    int getTotalLocations();

    int getLocationsSize();

    std::vector<Location> getLocations();

    std::vector<PRQuadTree *> getNodes();

    std::string getId() {
        return id;
    }

    void getLocationsInBounds(std::vector<Location> &returnNodes, BoundingBox box);

    void getNodeByCoordinate(std::vector<Location> &returnLocations, Coordinate coord);


};


#endif //INC_8042_PROJECT_PRQUADTREE_H
