#ifndef INC_8042_PROJECT_PRQUADTREE_H
#define INC_8042_PROJECT_PRQUADTREE_H


#include <string>

struct Bounds {
    float minLat;
    float maxLat;
    float minLong;
    float maxLong;
};

struct Coordinate {
    float latitude;
    float longitude;
};


class PRQuadTree {
public:
    void setBoundary(Bounds bounds);

    std::string toString(int level);

};


#endif //INC_8042_PROJECT_PRQUADTREE_H
