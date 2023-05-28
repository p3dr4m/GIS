#include "SystemManager.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

DMS SystemManager::fillDMS(string value) {
    DMS dms{};

    int degreeSize;
    int minSecSize = 2;
    int charSize = 1;

    if (value.length() == 8) {
        degreeSize = 3;

        dms.degrees = stoi(value.substr(0, degreeSize));
        dms.minutes = stoi(value.substr(3, minSecSize));
        dms.seconds = stoi(value.substr(5, minSecSize));
        dms.direction = (char) ((value.substr(7, charSize))[0]);

    } else if (value.length() == 7) {
        degreeSize = 2;

        dms.degrees = stoi(value.substr(0, degreeSize));
        dms.minutes = stoi(value.substr(2, minSecSize));
        dms.seconds = stoi(value.substr(4, minSecSize));
        dms.direction = (char) ((value.substr(6, charSize))[0]);
    } else {
        throw invalid_argument("Invalid DMS format");
    }

    return dms;
}

vector<vector<string>> SystemManager::readScript(string filename) {
    ifstream input(filename);
    string line;
    vector<vector<string>> commands;
    while (getline(input, line)) {
        istringstream iss(line);
        vector<string> arguments;
        string argument;

        // split line by tab
        while (getline(iss, argument, '\t')) {
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
