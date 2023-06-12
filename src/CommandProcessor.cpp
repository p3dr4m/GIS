#include "CommandProcessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "SystemManager.h"
#include "CoordinateIndex.h"

using namespace std;

void CommandProcessor::parseCmdScript(const string &filename) {
    vector<vector<string>> commands = systemManager.readScript(filename);
    Command command;
    for (auto &&arguments: commands) {
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
            throw invalid_argument("Invalid command: " + arguments[0]);
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
void CommandProcessor::worldCmd(vector<string> arguments) {
    if (arguments.size() != 5) {
        throw invalid_argument("Invalid world command\nUsage: world <westLong> <eastLong> <southLat> <northLat>");
    }
    cout << "world command" << endl;

    float westLong = DMS(arguments[1]).toFloat();
    float eastLong = DMS(arguments[2]).toFloat();
    float southLat = DMS(arguments[3]).toFloat();
    float northLat = DMS(arguments[4]).toFloat();

    gisRecord.setBounds(southLat, northLat, westLong, eastLong);


}

/**
 * import<tab><relative path>
 * import<tab><GIS record file>
 *
 * @param arguments
 */
void CommandProcessor::importCmd(vector<string> arguments) {

    if (arguments.size() != 2) {
        throw invalid_argument("Invalid import command\n"
                               "Usage: import <filename>");
    }
    cout << "import command" << endl;
    cout << "filepath: " << arguments[1] << endl;

    // open the file
    ifstream input(arguments[1]);
    string line, word;
    vector<string> row;
    int countingLines = 0;
    // print out each line
    while (getline(input, line)) {
        int fileOffset = input.tellg();

        // skip first line in file
        if (countingLines == 0) {
            countingLines++;
            continue;
        }
        // split line using "|"
        stringstream s(line);
        while (getline(s, word, '|')) {
            row.push_back(word);
        }
        countingLines++;
        gisRecord.insertRecord(row, countingLines, fileOffset);
        row.clear();


    }
    gisRecord.printNodes();
}

/**
 * debug<tab><quad|hash|pool|world>
 * @param arguments
 */
void CommandProcessor::debugCmd(vector<string> arguments) {
    if (arguments.size() != 2) {
        throw invalid_argument("Invalid debug command\n"
                               "Usage: debug <quad|hash|pool|world>");
    }
    cout << "debug command" << endl;
    cout << "debug type: " << arguments[1] << endl;
    if (arguments[1] == "quad") {
        // print out the quadtree
    } else if (arguments[1] == "hash") {
        // print out the hash table
    } else if (arguments[1] == "pool") {
        // print out the memory pool
    } else if (arguments[1] == "world") {
        // print out the world
    } else {
        cerr << "Invalid debug type" << endl;
        // explain how to use the command
        cerr << "Usage: debug <quad|hash|pool|world>" << endl;
        // exit with error
        exit(1);
    }
}

/**
 * what_is_at<tab><latitude><space><longitude>.
 * @param arguments
 */
void CommandProcessor::whatIsAtCmd(vector<string> arguments) {
    if (arguments.size() != 3) {
        throw invalid_argument("Invalid what_is_at command\n"
                               "Usage: what_is_at <latitude> <longitude>");
    }

    float latDec = DMS(arguments[1]).toFloat();
    float lngDec = DMS(arguments[2]).toFloat();
    vector<int> recordOffsets = gisRecord.findRecords(latDec, lngDec);
}

/**
 * what_is<tab><feature name><tab><state abbreviation>
 * @param arguments
 */
void CommandProcessor::whatIsCmd(vector<string> arguments) {
    if (arguments.size() != 3) {

        throw invalid_argument("Invalid what_is command\n"
                               "Usage: what_is <feature name> <state abbreviation>");
    }
    cout << "what_is command" << endl;
    cout << "feature name: " << arguments[0] << endl;
    cout << "state abbreviation: " << arguments[1] << endl;
}

/**
 * required:
 * what_is_in<tab><latitude><space><longitude<tab><half-height><tab><half-width>
 * optional:
 * what_is_in<tab>-filter<tab><pop|water|structure><tab><latitude><space><longitude<tab><half-height><tab><half-width>
 * what_is_in<tab>-long<tab><latitude><space><longitude<tab><half-height><tab><half-width>
 * @param arguments
 */
void CommandProcessor::whatIsInCmd(vector<string> arguments) {
    // check if the number of arguments isn't greater than 5 and less than 7
    if (arguments.size() < 5 || arguments.size() > 7) {
        throw invalid_argument("Invalid what_is_in command\n"
                               "Usage: what_is_in <latitude> <longitude> <half-height> <half-width>");
    }

    string filterOption = "None";
    bool longLog = false;
    string latitude;
    string longitude;
    float halfHeight;
    float halfWidth;

    if (arguments[1] == "-filter") {
        if (arguments[2] == "pop" || arguments[2] == "water" || arguments[2] == "structure") {
            filterOption = arguments[2];
        } else {
            throw invalid_argument("Invalid what_is_in command\n"
                                   "Usage: what_is_in<tab>-filter<tab><pop|water|structure> <latitude> <longitude> <half-height> <half-width>");
        }

        latitude = arguments[3];
        longitude = arguments[4];
        halfHeight = stof(arguments[5]) / 3600.f;
        halfWidth = stof(arguments[6]) / 3600.f;
        float latDec = DMS(latitude).toFloat();
        float lngDec = DMS(longitude).toFloat();
        vector<int> records = gisRecord.findRecords(lngDec, latDec, halfWidth, halfHeight);

    } else if (arguments[1] == "-long") {
        longLog = true;
        latitude = arguments[2];
        longitude = arguments[3];
        halfHeight = stof(arguments[4]) / 3600.f;
        halfWidth = stof(arguments[5]) / 3600.f;
        float latDec = DMS(latitude).toFloat();
        float lngDec = DMS(longitude).toFloat();
        vector<int> records = gisRecord.findRecords(lngDec, latDec, halfWidth, halfHeight);
    } else {
        latitude = arguments[1];
        longitude = arguments[2];
        halfHeight = stof(arguments[3]) / 3600.f;
        halfWidth = stof(arguments[4]) / 3600.f;
        float latDec = DMS(latitude).toFloat();
        float lngDec = DMS(longitude).toFloat();

        vector<int> records = gisRecord.findRecords(lngDec, latDec, halfWidth, halfHeight);
    }
}


/**
 * quit<tab>
 */
void CommandProcessor::quitCmd() {
    cout << "quit command" << endl;
    cout << "Exiting..." << endl;
    exit(0);
}

