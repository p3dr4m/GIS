//
// Created by p3dr4 on 2023-07-08.
//

#ifndef INC_8042_PROJECT_HASHENTRY_H
#define INC_8042_PROJECT_HASHENTRY_H

#include <string>
#include <vector>

struct HashEntry {
    std::string featureName;
    std::string stateAbbreviation;
    std::vector<int> offsets;
    bool exists = false;
};
#endif //INC_8042_PROJECT_HASHENTRY_H
