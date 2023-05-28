//
// Created by Raziq on 2023-04-26.
//

#ifndef INC_8042_PROJECT_SYSTEMMANAGER_H
#define INC_8042_PROJECT_SYSTEMMANAGER_H

#include <string>
#include "GISRecord.h"

using namespace std;


class SystemManager {
public:
    DMS fillDMS(string value);
    vector<vector<string>> readScript(string filename);
};


#endif //INC_8042_PROJECT_SYSTEMMANAGER_H
