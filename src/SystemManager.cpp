#include "SystemManager.h"
#include "GISRecord.h"
#include <stdexcept>

GISRecord::DMS SystemManager::fillDMS(std::string value) {
    GISRecord::DMS dms{};

    int degreeSize;
    int minSecSize = 2;
    int charSize = 1;

    if (value.length() == 8) {
        degreeSize = 3;

        dms.degrees = std::stoi(value.substr(0, degreeSize));
        dms.minutes = std::stoi(value.substr(3, minSecSize));
        dms.seconds = std::stoi(value.substr(5, minSecSize));
        dms.direction = (char)((value.substr(7, charSize))[0]);

    } else if (value.length() == 7) {
        degreeSize = 2;

        dms.degrees = std::stoi(value.substr(0, degreeSize));
        dms.minutes = std::stoi(value.substr(2, minSecSize));
        dms.seconds = std::stoi(value.substr(4, minSecSize));
        dms.direction = (char)((value.substr(6, charSize))[0]);
    } else {
        throw std::invalid_argument("Invalid DMS format");
    }

    return dms;
}