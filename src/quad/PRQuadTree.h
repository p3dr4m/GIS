#ifndef INC_8042_PROJECT_PRQUADTREE_H
#define INC_8042_PROJECT_PRQUADTREE_H


#include "../structs/BoundingBox.h"
#include "../structs/Location.h"
#include <string>
#include <limits>
#include <memory>
#include <vector>


/**
 * https://pvigier.github.io/2019/08/04/quadtree-collision-detection.html
 */
class PRQuadTree {
private:
    int MAX_NODES = 4; // max number of nodes in a leaf node
    int MAX_LEVELS = 999999999; // max number of levels in the tree
    int level = 0; // the current level of the tree
    std::vector<Location> locations; // the locations in the tree
    BoundingBox boundingBox; // the bounding box of the tree
    std::vector<std::unique_ptr<PRQuadTree>> nodes; // the subtrees of the tree
    std::string id; // the id of the tree

    static bool nearlyEqual(float a, float b, float epsilon) {
        float absA = std::fabs(a);
        float absB = std::fabs(b);
        float diff = std::fabs(a - b);

        if (a == b) {
            return true;
        } else if (a == 0 || b == 0 || diff < std::numeric_limits<float>::min()) {
            return diff < epsilon;
        } else {
            return diff / (absA + absB) < epsilon;
        }
    }

    static bool isBoxIntersect(BoundingBox box1, BoundingBox box2) {
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

public:
    PRQuadTree(int pLevel, BoundingBox pBox, std::string _id) : level(pLevel), boundingBox(pBox), id(std::move(_id)) {
        nodes.resize(4);
    };


    bool isLeaf() {
        return nodes[0] == nullptr;
    }

    void clear();

    void split();

    int getIndex(const Location &location) const;;

    bool insert(const Location &location);

    std::vector<Location> retrieve(std::vector<Location> returnNodes, const Location &location);

    static bool isCoordInBox(Coordinate coord, BoundingBox box);

    bool isCoordInBox(Coordinate coord);

    int getTotalLocations();

    int getLocationsSize() {
        return (int) locations.size();
    }

    std::vector<std::unique_ptr<PRQuadTree>> &getNodes() {
        return nodes;
    }

    std::vector<Location> getLocations() {
        return locations;
    }

    std::string getId() {
        return id;
    }

    void getLocationsInBounds(std::vector<Location> &returnNodes, BoundingBox box);

    void getNodeByCoordinate(std::vector<Location> &returnLocations, Coordinate coord);

    Location &find(Coordinate coord);
};


#endif //INC_8042_PROJECT_PRQUADTREE_H
