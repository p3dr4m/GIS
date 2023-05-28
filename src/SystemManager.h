//
// Created by Raziq on 2023-04-26.
//

#ifndef FINALPROJALGO_SYSTEMMANAGER_H
#define FINALPROJALGO_SYSTEMMANAGER_H

#include <string>
#include "GISRecord.h"

using namespace std;

class SystemManager {
    public:
    GISRecord::DMS fillDMS(string value);
};


#endif //FINALPROJALGO_SYSTEMMANAGER_H
