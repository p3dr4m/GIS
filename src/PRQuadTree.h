
#ifndef INC_8042_PROJECT_PRQUADTREE_H
#define INC_8042_PROJECT_PRQUADTREE_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

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

    string toString(int level);

};


#endif //INC_8042_PROJECT_PRQUADTREE_H
