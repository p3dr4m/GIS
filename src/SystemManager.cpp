#include "SystemManager.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

DMS SystemManager::fillDMS(std::string value) {
    DMS dms{};

    int degreeSize;
    int minSecSize = 2;
    int charSize = 1;

    if (value.length() == 8) {
        degreeSize = 3;

        dms.degrees = std::stoi(value.substr(0, degreeSize));
        dms.minutes = std::stoi(value.substr(3, minSecSize));
        dms.seconds = std::stoi(value.substr(5, minSecSize));
        dms.direction = (char) ((value.substr(7, charSize))[0]);

    } else if (value.length() == 7) {
        degreeSize = 2;

        dms.degrees = std::stoi(value.substr(0, degreeSize));
        dms.minutes = std::stoi(value.substr(2, minSecSize));
        dms.seconds = std::stoi(value.substr(4, minSecSize));
        dms.direction = (char) ((value.substr(6, charSize))[0]);
    } else {
        throw std::invalid_argument("Invalid DMS format");
    }

    return dms;
}

vector<vector<std::string>> SystemManager::readScript(string filename) {
    std::ifstream input(filename);
    std::string line;
    std::vector<std::vector<std::string>> commands;
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        std::vector<std::string> arguments;
        std::string argument;

        // split line by tab
        while (std::getline(iss, argument, '\t')) {
            arguments.push_back(argument);
        }

        if (arguments.empty() || arguments[0].empty() || arguments[0][0] == ';') {
            // empty or comment line, skip
            continue;
        }

        commands.push_back(arguments);
    }
    return commands;
}
