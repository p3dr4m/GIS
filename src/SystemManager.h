

#ifndef INC_8042_PROJECT_SYSTEMMANAGER_H
#define INC_8042_PROJECT_SYSTEMMANAGER_H

#include <string>
#include <vector>
#include "GISRecord.h"

class SystemManager {
public:
    DMS fillDMS(std::string value);

    std::vector<std::vector<std::string>> readScript(std::string filename);
};


#endif //INC_8042_PROJECT_SYSTEMMANAGER_H
