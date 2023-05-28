#include "CommandProcessor.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace std;

void CommandProcessor::parseCmdScript(const std::string &filename) {
    vector<vector<string>> commands = systemManager.readScript(filename);
    Command command;
    for (auto &&arguments: commands) {
        // convert command to enum
        if (arguments[0] == "world") {
            command = world;
        } else if (arguments[0] == "import") {
            command = import;
        } else if (arguments[0] == "debug") {
            command = debug;
        } else if (arguments[0] == "what_is_at") {
            command = what_is_at;
        } else if (arguments[0] == "what_is") {
            command = what_is;
        } else if (arguments[0] == "what_is_in") {
            command = what_is_in;
        } else if (arguments[0] == "quit") {
            command = quit;
        } else {
            throw std::invalid_argument("Invalid command: " + arguments[0]);
        }

        // process command
        switch (command) {
            case world:
                worldCmd(arguments);
                break;
            case import:
                importCmd(arguments);
                break;
            case debug:
                debugCmd(arguments);
                break;
            case what_is_at:
                whatIsAtCmd(arguments);
                break;
            case what_is:
                whatIsCmd(arguments);
                break;
            case what_is_in:
                whatIsInCmd(arguments);
                break;
            case quit:
                quitCmd();
                break;
        }
    }
}


/**
 * world<tab><westLong><tab><eastLong><tab><southLat><tab><northLat>
 * @param arguments
 */
void CommandProcessor::worldCmd(std::vector<std::string> arguments) {
    if (arguments.size() != 5) {
        throw std::invalid_argument("Invalid world command\nUsage: world <westLong> <eastLong> <southLat> <northLat>");
    }
    std::cout << "world command" << std::endl;
    DMS westLong{}, eastLong{}, northLat{}, southLat{};

    westLong = systemManager.fillDMS(arguments[1]);
    eastLong = systemManager.fillDMS(arguments[2]);
    southLat = systemManager.fillDMS(arguments[3]);
    northLat = systemManager.fillDMS(arguments[4]);

    float w = gisRecord.convertDMS(westLong);
    float e = gisRecord.convertDMS(eastLong);
    float s = gisRecord.convertDMS(southLat);
    float n = gisRecord.convertDMS(northLat);


}

/**
 * import<tab><relative path>
 * import<tab><GIS record file>
 *
 * @param arguments
 */
void CommandProcessor::importCmd(std::vector<std::string> arguments) {
    if (arguments.size() != 2) {
        std::cerr << "Invalid import command" << std::endl;
        // explain how to use the command
        std::cerr << "Usage: import <filename>" << std::endl;
        // exit with error
        exit(1);
    }
    std::cout << "import command" << std::endl;
    std::cout << "filepath: " << arguments[1] << std::endl;

    // open the file
    std::ifstream input(arguments[1]);
    std::string line;
    // print out each line
    while (std::getline(input, line)) {
        break;
    }
}

/**
 * debug<tab><quad|hash|pool|world>
 * @param arguments
 */
void CommandProcessor::debugCmd(std::vector<std::string> arguments) {
    if (arguments.size() != 2) {
        std::cerr << "Invalid debug command" << std::endl;
        // explain how to use the command
        std::cerr << "Usage: debug <quad|hash|pool|world>" << std::endl;
        // exit with error
        exit(1);
    }
    std::cout << "debug command" << std::endl;
    std::cout << "debug type: " << arguments[1] << std::endl;
    if (arguments[1] == "quad") {
        // print out the quadtree
    } else if (arguments[1] == "hash") {
        // print out the hash table
    } else if (arguments[1] == "pool") {
        // print out the memory pool
    } else if (arguments[1] == "world") {
        // print out the world
    } else {
        std::cerr << "Invalid debug type" << std::endl;
        // explain how to use the command
        std::cerr << "Usage: debug <quad|hash|pool|world>" << std::endl;
        // exit with error
        exit(1);
    }
}

/**
 * what_is_at<tab><latitude><space><longitude>.
 * @param arguments
 */
void CommandProcessor::whatIsAtCmd(std::vector<std::string> arguments) {
    // print arguments

    if (arguments.size() != 3) {
        std::cerr << "Invalid what_is_at command" << std::endl;
        // explain how to use the command
        std::cerr << "Usage: what_is_at <latitude> <longitude>" << std::endl;
        // exit with error
        exit(1);
    }
    std::cout << "what_is_at command" << std::endl;
    std::cout << "latitude: " << arguments[1] << std::endl;
    std::cout << "longitude: " << arguments[2] << std::endl;
}

/**
 * what_is<tab><feature name><tab><state abbreviation>
 * @param arguments
 */
void CommandProcessor::whatIsCmd(std::vector<std::string> arguments) {
    if (arguments.size() != 3) {
        std::cerr << "Invalid what_is command" << std::endl;
        // explain how to use the command
        std::cerr << "Usage: what_is <feature name> <state abbreviation>" << std::endl;
        // exit with error
        exit(1);
    }
    std::cout << "what_is command" << std::endl;
    std::cout << "feature name: " << arguments[0] << std::endl;
    std::cout << "state abbreviation: " << arguments[1] << std::endl;
}

/**
 * required:
 * what_is_in<tab><latitude><space><longitude<tab><half-height><tab><half-width>
 * optional:
 * what_is_in<tab>-filter<tab><pop|water|structure><tab><latitude><space><longitude<tab><half-height><tab><half-width>
 * what_is_in<tab>-long<tab><latitude><space><longitude<tab><half-height><tab><half-width>
 * @param arguments
 */
void CommandProcessor::whatIsInCmd(std::vector<std::string> arguments) {
    // check if the number of arguments isn't greater than 5 and less than 7
    if (arguments.size() < 5 || arguments.size() > 7) {
        std::cerr << "Invalid what_is_in command" << std::endl;
        // explain how to use the command
        std::cerr << "Usage: what_is_in <latitude> <longitude> <half-height> <half-width>" << std::endl;
        // exit with error
        exit(1);
    }

    std::string filterOption = "None";
    bool longLog = false;
    std::string latitude;
    std::string longitude;
    int halfHeight;
    int halfWidth;

    if (arguments[1] == "-filter") {
        ///  -filter [ pop | water | structure ]
        if (arguments[2] == "pop" || arguments[2] == "water" || arguments[2] == "structure") {
            filterOption = arguments[2];
        } else {
            std::cerr << "Invalid filter option" << std::endl;
            // explain how to use the command
            std::cerr
                    << "Usage: what_is_in<tab>-filter<tab><pop|water|structure> <latitude> <longitude> <half-height> <half-width>"
                    << std::endl;
            // exit with error
            exit(1);
        }

        latitude = arguments[3];
        longitude = arguments[4];
        halfHeight = std::stoi(arguments[5]);
        halfWidth = std::stoi(arguments[6]);

    } else if (arguments[1] == "-long") {
        longLog = true;
        latitude = arguments[2];
        longitude = arguments[3];
        halfHeight = std::stoi(arguments[4]);
        halfWidth = std::stoi(arguments[5]);
    } else {
        latitude = arguments[1];
        longitude = arguments[2];
        halfHeight = std::stoi(arguments[3]);
        halfWidth = std::stoi(arguments[4]);
    }

    std::cout << "what_is_in command" << std::endl;
    std::cout << "filter option: " << filterOption << std::endl;
    std::cout << "long log: " << longLog << std::endl;
    std::cout << "latitude: " << latitude << std::endl;
    std::cout << "longitude: " << longitude << std::endl;
    std::cout << "half-height: " << halfHeight << std::endl;
    std::cout << "half-width: " << halfWidth << std::endl;
}


/**
 * quit<tab>
 */
void CommandProcessor::quitCmd() {
    std::cout << "quit command" << std::endl;
    std::cout << "Exiting..." << std::endl;
    exit(0);
}

