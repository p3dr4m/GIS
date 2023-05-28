
#ifndef INC_8042_PROJECT_PRQUADTREE_H
#define INC_8042_PROJECT_PRQUADTREE_H

#include <string>
#include <vector>
#include <iostream>
#include "GISRecord.h"

class PRQuadTree {
public:
    void setBoundary(GISRecord::Bounds bounds);
    struct Coord {
        float latitude;
        float longitude;
    };

    struct Node {
        Coord bound;

    };

    struct BoundingBox {

    };
private:

};


#endif //INC_8042_PROJECT_PRQUADTREE_H
