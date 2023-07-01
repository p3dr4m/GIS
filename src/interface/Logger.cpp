#include "Logger.h"
#include "SystemManager.h"
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;

string Logger::getTime() {
    char s[1000];

    time_t t = time(nullptr);
    struct tm *p = localtime(&t);

    strftime(s, 1000, "%a %b %d %H:%M:%S %Z %Y", p);
    return s;
}

void Logger::headerLog(const string &databaseFilePath, const string &cmdScriptFilePath,
                       const string &logFilePath) {
    // write to the logFile ofstream from SystemManager
    vector<string> lines;


    lines.emplace_back("Course Project for COMP 8042");
    lines.emplace_back("Student Name: Raziq Khan, Student Id: A00990021");
    lines.emplace_back("Student Name: Pedram Nazari, Student Id: A00931203");
    lines.emplace_back("Begin of GIS Program log:");
    lines.push_back("dbFile:\t" + databaseFilePath);
    lines.push_back("script:\t" + cmdScriptFilePath);
    lines.push_back("log:\t" + logFilePath);
    lines.push_back("Start Time: " + getTime());

    // write to the logFile ofstream from SystemManager

    SystemManager::writeLinesToFile(logFile, lines);
//    SystemManager::closeFile(logFile);
}

void Logger::worldLog(const vector<string> &arguments) {
    DMS westLong = DMS(arguments[1]);
    DMS eastLong = DMS(arguments[2]);
    DMS southLat = DMS(arguments[3]);
    DMS northLat = DMS(arguments[4]);
    vector<string> lines;
    if (cmdCount == 0) {
        string comment = "; Script 1\n"
                         ";\n"
                         "; Testing using a small dataset\n"
                         ";\n"
                         "; Specify boundaries of coordinate space:\n"
                         ";";
        lines.push_back(comment);
    }
    string cmd = "world\t" + arguments[1] + "\t" + arguments[2] + "\t" + arguments[3] + "\t" + arguments[4] + "\n";
    lines.push_back(cmd);
    lines.push_back(separator);
    lines.emplace_back("Latitude/longitude values in index entries are shown as signed integers, in total seconds.");
    lines.push_back(separator);
    string sixTabs(6, '\t');
    string elevenSpaces(11, ' ');
    string fourteenSpaces(14, ' ');

    string logLine = sixTabs + "World boundaries are set to:\n" +
                     sixTabs + elevenSpaces + northLat.toTotalSeconds() +
                     "\n" + sixTabs + westLong.toTotalSeconds() +
                     fourteenSpaces + eastLong.toTotalSeconds() +
                     "\n" + sixTabs + elevenSpaces + southLat.toTotalSeconds();
    lines.push_back(logLine);
    lines.push_back(separator);


    SystemManager::writeLinesToFile(logFile, lines);
    cmdCount++;
}

void Logger::importLog(const vector<string> &arguments, vector<int> data) {
    vector<string> lines;

    if (cmdCount == 1) {
        string comment = ";\n"
                         "; Import the dataset [the address here is a relative address]";
        lines.push_back(comment);
    }
    string cmd = "Command " + to_string(cmdCount) + ": " + "import\t" + arguments[1] + "\n";
    lines.push_back(cmd);

    string output = "Imported Features by name: " + to_string(data[0]) + "\n" +
                    "Longest probe sequence:   " + to_string(data[1]) + "\n" +
                    "Imported Locations:        " + to_string(data[2]) + "\n" +
                    "Average name length:       " + to_string(data[3]);
    lines.push_back(output);
    lines.push_back(separator);
    SystemManager::writeLinesToFile(logFile, lines);
    cmdCount++;
}

/**
 * Log the contents of the specified index structure in a fashion that makes the
 * internal structure and contents of the index clear. It is not necessary to be overly
 * verbose here, but you should include information like key values and file offsets
 * where a p p r o p r i a t e .
 * Hint: for d e b u g < t a b > w o r l d t hi n k of a recursive m e t h o d that c a n divide the 2-D
 * world space in 4 parts each time and continue the recursion for a number of times.
 * @param option
 * @param tree
 */
void Logger::debugWorld(const vector<string> &option, PRQuadTree &tree) {
    vector<string> lines;
    if (cmdCount == 2) {
        string comment = ";\n"
                         "; Lets visualize our already imported geographical data";
        lines.push_back(comment);
    }
    string cmd = "Command " + to_string(cmdCount) + ": " + "debug\t" + option[1] + "\n";
    lines.push_back(cmd);

    printWorld(tree, lines);
    lines.push_back(separator);
    SystemManager::writeLinesToFile(logFile, lines);
    cmdCount++;
}

void Logger::printWorld(PRQuadTree &tree, vector<string> &lines) {
    // create an empty grid
    vector<vector<int>> grid(40, vector<int>(150));

    vector<Location> nodes;

    // print the grid
    // add a line of dashes to the beginning and end of the grid. the dash lines also have + at the beginning and end
    string dashLine = "+";
    for (int i = 0; i < 150; i++) {
        dashLine += "-";
    }
    dashLine += "+";
    lines.push_back(dashLine);


    recurseTree(tree, grid, 0, 140, 0, 40);  // update grid with values from quad tree

    // reverse the grid and the nested vectors
    reverse(grid.begin(), grid.end());

    // reve

    for (const auto &row: grid) {
        string rowStr;
        // add | to the beginning of the row and | to the end of the row
        rowStr += "|";
        for (int node: row) {
            if (node == 0)
                rowStr += " ";
            else
                rowStr += to_string(node);
        }
        rowStr += "|";
        lines.push_back(rowStr);
    }

    lines.push_back(dashLine);
}

void Logger::recurseTree(PRQuadTree &tree, vector<vector<int>> &grid, int minX, int maxX, int minY, int maxY) {
    int midX = (maxX + minX) / 2;
    int midY = (maxY + minY) / 2;

    if (tree.isLeaf()) {
        grid[midY][midX] = tree.getLocationsSize();
    } else {
        int childBoundsX[4][2] = {{midX, maxX},
                                  {midX, maxX},
                                  {minX, midX},
                                  {minX, midX}};
        int childBoundsY[4][2] = {{midY, maxY},
                                  {minY, midY},
                                  {midY, maxY},
                                  {minY, midY}};

        for (int i = 0; i < 4; ++i) {
            if (tree.getNodes()[i] != nullptr) {
                recurseTree(*tree.getNodes()[i], grid,
                            childBoundsX[i][0], childBoundsX[i][1],
                            childBoundsY[i][0], childBoundsY[i][1]);
            }
        }
    }
}


void Logger::debugQuad(const vector<string> &option, PRQuadTree &tree) {
    vector<string> lines;

    printDebugQuad(lines, tree, 0);
    lines.push_back(separator);

    SystemManager::writeLinesToFile(logFile, lines);
    cmdCount++;

}


void Logger::printDebugQuad(vector<string> &lines, PRQuadTree &tree, int depth = 0) {
    string padding(depth * 3, ' '); // three spaces for each level of depth

    if (tree.isLeaf()) {
        if (!tree.getLocations().empty()) {
            string bufferLine;
            for (auto &location: tree.getLocations()) {
                string databaseLine;
                for (const auto &line: location.getDatabaseLine()) {
                    databaseLine += to_string(line) + "|";
                }
                if (!databaseLine.empty()) {
                    databaseLine.pop_back();
                }
                string line =
                        "[(" + to_string(location.getYInSec()) + "," + to_string(location.getXInSec()) +
                        "), " + databaseLine + "] ";
                bufferLine += line;
            }
            lines.push_back(padding + tree.getId() + bufferLine);
        } else {
            lines.push_back(padding + tree.getId() + "*");
        }
    } else {
        // Node indicator @ added here
        lines.push_back(padding + tree.getId() + "@");
        for (int i = 0; i < 4; ++i) {
            if (tree.getNodes()[i] != nullptr) {
                printDebugQuad(lines, *tree.getNodes()[i], depth + 1);
            }
        }
    }
}


void Logger::whatIsInLog(vector<string> arguments, vector<Record> records) {
    //  The following 7 feature(s) were found in (38d 28m 12s North +/- 60, 79d 31m 56s West +/- 90)
    //
    //	10:  "Forks of Waters"  "VA"  "(38d 28m 56s North, 79d 30m 31s West)
    //	34:  "Strait Creek"  "VA"  "(38d 28m 56s North, 79d 30m 31s West)
    //	47:  "Possum Trot"  "VA"  "(38d 28m 12s North, 79d 31m 56s West)
    //	12:  "Ginseng Mountain"  "VA"  "(38d 28m 50s North, 79d 31m 39s West)
    //	22:  "Laurel Run"  "VA"  "(38d 27m 25s North, 79d 31m 59s West)
    //	28:  "Peck Run"  "VA"  "(38d 28m 6s North, 79d 31m 9s West)
    //	41:  "Wooden Run"  "VA"  "(38d 27m 18s North, 79d 32m 1s West)

    string cmdStr = "Command " + to_string(cmdCount) + ": what_is_in\t" + arguments[1] + "\t" + arguments[2] + "\t" + arguments[3] + "\t" + arguments[4] + "\n";

    SystemManager::writeLineToFile(logFile, cmdStr);

    vector<string> lines;
    DMS lat{};
    DMS lng{};
    string filterOption;
    if (arguments[1] == "-filter") {
        filterOption = arguments[2];
        lat = DMS(arguments[3]);
        lng = DMS(arguments[4]);
    } else if (arguments[1] == "-long") {
        lng = DMS(arguments[2]);
        lat = DMS(arguments[3]);
    } else {
        lat = DMS(arguments[1]);
        lng = DMS(arguments[2]);
    }


    if (records.empty()) {
        //Nothing was found in ("38d 20m 12s North +/- 60", "79d 23m 30s West +/- 90")
        string firstLine = "Nothing was found in (" + arguments[1] + ", " + arguments[2] +")\n";
    } else {
        // print the first line in the comment above
        string firstLine = "The following " + to_string(records.size()) + " feature(s) were found in " +
                           arguments[1] + "\n";
        lines.push_back(firstLine);

        // sort records by record.offset
        sort(records.begin(), records.end(), [](const Record &a, const Record &b) {
            return a.offset < b.offset;
        });
        for (const Record &record: records) {
            vector<string> row = record.getRowVector();
            DMS latDMS = DMS(row[PRIMARY_LAT_DMS]);
            DMS lngDMS = DMS(row[PRIM_LONG_DMS]);
            string line = "\t" + to_string(record.offset)  + "  " +  record.str() + "\n";
            lines.push_back(line);
        }

    }

    SystemManager::writeLinesToFile(logFile, lines);
    cmdCount++;
}

void Logger::whatIsAtLog(vector<string> arguments, vector<Record> records, vector<int> offsets) {
    stringstream whatIsAtLogStr;

    whatIsAtLogStr << "Command " << cmdCount << ": what_is_at\t" << arguments[1] << "\t" << arguments[2] << "\n\n";

    //check for empty records
    if (records.empty()) {
        whatIsAtLogStr << "Nothing was found at (" << arguments[1] << ", " << arguments[2] << ")\n";
    } else {
        for (size_t i = 0; i < records.size(); ++i) {
            whatIsAtLogStr << "\t" << offsets[i] << ":  " << records[i].str() << "\n";
        }
    }


    SystemManager::writeLineToFile(logFile, whatIsAtLogStr.str());
    cmdCount++;

}

void Logger::debugPool(BufferPool<Record> bufferPool) {
    string bufferPoolStr;
    // add "BufferPool: " to the beginning of the string
    bufferPoolStr += "Command " + to_string(cmdCount) + ": debug\tpool\n";
    bufferPoolStr += bufferPool.str();

    SystemManager::writeLineToFile(logFile, bufferPoolStr);
    cmdCount++;
}

int Logger::logToDatabase(const string &line, int offset) {
    // concat all the strings in the row vector
    offset = (int) SystemManager::writeLineToFile(databaseFile, line);
    return offset;
}

void Logger::whatIsLog(vector<string> arguments, vector<Record> records, vector<int> offsets) {
    string logStr;
    openDbFile();
    string cmdStr = "Command " + to_string(cmdCount) + ": what_is\t" + arguments[1] + "\t" + arguments[2] + "\n";

    SystemManager::writeLineToFile(logFile, cmdStr);
    if (records.empty()) {
        logStr = "No records match " + arguments[1] + ", " + arguments[2];
        SystemManager::writeLineToFile(logFile, logStr);
    } else {
        for (int i = 0; i < records.size(); i++) {
            vector<string> row = records[i].getRowVector();
            DMS latDMS = DMS(row[PRIMARY_LAT_DMS]);
            DMS lngDMS = DMS(row[PRIM_LONG_DMS]);
            logStr =  "  " + to_string(offsets[i]) + ": " + row[COUNTY_NAME] + " " + "(" + latDMS.toLogString() + ", " +
                     lngDMS.toLogString() + ")";

            SystemManager::writeLineToFile(logFile, logStr);
        }
    }
    // new line
    SystemManager::writeLineToFile(logFile, separator);
    closeDbFile();
    cmdCount++;
}

void Logger::debugHash(const std::string& hashTableStr) {
    string debugHashStr;
    debugHashStr += "Command " + to_string(cmdCount) + ": debug\thash\n";
    debugHashStr += hashTableStr;

    SystemManager::writeLineToFile(logFile, debugHashStr);
    cmdCount++;
}
