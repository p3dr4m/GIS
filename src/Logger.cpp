#include <chrono>
#include <vector>
#include "Logger.h"
#include "SystemManager.h"
#include <iostream>
using namespace std;

void Logger::logHeader(const string &databaseFilePath, const string &cmdScriptFilePath, const string &logFilePath) {
    // write to the logFile ofstream from SystemManager
    vector<string> lines;


    lines.emplace_back("Course Project for COMP 8042");
    lines.emplace_back("Student Name: Raziq Khan, Student Id: A");
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

void Logger::importLog(const std::vector<std::string> &arguments, std::vector<int> data) {
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

string Logger::getTime() {
    char s[1000];

    time_t t = time(nullptr);
    struct tm * p = localtime(&t);

    strftime(s, 1000, "%a %b %d %H:%M:%S %Z %Y", p);
    return s;
}
