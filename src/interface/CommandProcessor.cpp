#include <fstream>
#include <stdexcept>
#include <utility>
#include "CommandProcessor.h"
#include "Logger.h"

using namespace std;

void CommandProcessor::runCommand(const vector<string> &arguments) {
    Command command;
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
            quitCmd(arguments);
            break;
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

    DMS westLong = DMS(arguments[1]);
    DMS eastLong = DMS(arguments[2]);
    DMS southLat = DMS(arguments[3]);
    DMS northLat = DMS(arguments[4]);
    float westLongDec = westLong.toFloat();
    float eastLongDec = eastLong.toFloat();
    float southLatDec = southLat.toFloat();
    float northLatDec = northLat.toFloat();
    string westLongSecs = westLong.toTotalSeconds();
    string eastLongSecs = eastLong.toTotalSeconds();
    string southLatSecs = southLat.toTotalSeconds();
    string northLatSecs = northLat.toTotalSeconds();
    Logger &logger = Logger::getInstance();
    logger.worldLog(arguments);

    gisRecord.setBounds(southLatDec, northLatDec, westLongDec, eastLongDec);


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

    // open the file
    ifstream input(arguments[1]);
    if (!input.is_open()) {
        throw invalid_argument("Unable to open file: " + arguments[1]);
    }
    string line, word;
    int countingLines = -1;
    // Use SystemManager to read the file
    Logger &logger = Logger::getInstance();
    // reading the file from import command and inserting into the quadtree
    logger.openDbFile();
    string nameLengths;
    SystemManager::readDatabase(arguments[1], [&](const string& line) {
        // skip first line in file
        if (countingLines == -1) {
            countingLines++;
            return;
        }

        Record record = Record(line);
        vector<string> row = record.getRow();
        Coordinate coord = Coordinate(stof(row[PRIM_LONG_DEC]), stof(row[PRIM_LAT_DEC]));
        bool inserted = gisRecord.getTree().isCoordInBox(coord);

        if (inserted) {
            int offset = logger.logToDatabase(line);
            gisRecord.insertRecord(row, countingLines, offset);
            nameLengths += row[FEATURE_NAME];
            countingLines++;
        }
    });
    logger.closeDbFile();
    int nodeCount = gisRecord.getNodeCount();
    int importedNames = gisRecord.getImportedNames();
    int longestProbe = gisRecord.getLongestProbe();
    unsigned long avgNameLength = nameLengths.size() / nodeCount;
    vector<int> data = {importedNames, longestProbe, nodeCount, static_cast<int>(avgNameLength)};
    logger.importLog(arguments, data);
    gisRecord.dbFileName = arguments[1];
}

/**
 * debug<tab><quad|hash|pool|world>
 * @param arguments
 */
void CommandProcessor::debugCmd(vector<string> arguments) {
    if (arguments.size() != 2 || (!(arguments[1] == "quad"
                                    || arguments[1] == "hash" || arguments[1] == "pool" ||
                                    arguments[1] == "world"))) {
        throw invalid_argument("Invalid debug command\n"
                               "Usage: debug <quad|hash|pool|world>");
    }
    PRQuadTree &quadTree = gisRecord.getTree();
    string hashStr = gisRecord.getHashTableStr();


    int nodeCount = quadTree.getTotalLocations();

    Logger &logger = Logger::getInstance();
    if (arguments[1] == "quad") {
        // print out the quadtree
        logger.debugQuad(arguments, quadTree);
    } else if (arguments[1] == "hash") {
        // print out the hash table
        logger.debugHash(hashStr);
    } else if (arguments[1] == "pool") {
        // print out the memory pool
        BufferPool<Record> &pool = gisRecord.getBuffer();
        logger.debugPool(pool);
    } else if (arguments[1] == "world") {
        // print out the world
        logger.debugWorld(arguments, quadTree);

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
    vector<int> offsets = gisRecord.findRecords(latDec, lngDec);
    Logger &logger = Logger::getInstance();
    string logString;
    vector<Record> records = gisRecord.getRecords(offsets);


    logger.whatIsAtLog(arguments, records, offsets);
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
    string featureName = arguments[1];
    string stateAbbreviation = arguments[2];

    vector<int> offsets = gisRecord.findRecords(featureName, stateAbbreviation);
    Logger &logger = Logger::getInstance();
    vector<Record> records = gisRecord.getRecords(offsets);

    logger.whatIsLog(arguments, records, offsets);
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

    string filterOption;
    string latitude;
    string longitude;
    float halfHeight;
    float halfWidth;
    float latDec;
    float lngDec;
    vector<int> offsets;
    vector<Record> records;
    Logger &logger = Logger::getInstance();
    if (arguments[1] == "-filter") {
        if (!(arguments[2] == "pop" || arguments[2] == "water" || arguments[2] == "structure")) {
            throw invalid_argument("Invalid what_is_in command\n"
                                   "Usage: what_is_in<tab>-filter<tab><pop|water|structure>"
                                   " <latitude> <longitude> <half-height> <half-width>");
        }

        filterOption = arguments[2];
        latitude = arguments[3];
        longitude = arguments[4];
        halfHeight = stof(arguments[5]) / 3600.f;
        halfWidth = stof(arguments[6]) / 3600.f;
        latDec = DMS(latitude).toFloat();
        lngDec = DMS(longitude).toFloat();
        offsets = gisRecord.findRecords(lngDec, latDec, halfWidth, halfHeight);
        records = gisRecord.getRecords(offsets);
        logger.whatIsInLog(arguments, records);
    } else if (arguments[1] == "-long") {
        latitude = arguments[2];
        longitude = arguments[3];
        halfHeight = stof(arguments[4]) / 3600.f;
        halfWidth = stof(arguments[5]) / 3600.f;
        latDec = DMS(latitude).toFloat();
        lngDec = DMS(longitude).toFloat();
        offsets = gisRecord.findRecords(lngDec, latDec, halfWidth, halfHeight);
        records = gisRecord.getRecords(offsets);
        logger.whatIsInLog(arguments, records);
    } else {
        latitude = arguments[1];
        longitude = arguments[2];
        halfHeight = stof(arguments[3]) / 3600.f;
        halfWidth = stof(arguments[4]) / 3600.f;
        DMS latDms = DMS(latitude);
        latDec = latDms.toFloat();
        lngDec = DMS(longitude).toFloat();
        offsets = gisRecord.findRecords(lngDec, latDec, halfWidth, halfHeight);
        records = gisRecord.getRecords(offsets);
        logger.whatIsInLog(arguments, records);
    }
}


/**
 * quit<tab>
 */
void CommandProcessor::quitCmd(vector<string> arguments) {
    // logger close files
    Logger &logger = Logger::getInstance();
    logger.quitCmd(std::move(arguments));
    exit(0);
}


