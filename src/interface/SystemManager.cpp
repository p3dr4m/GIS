#include "SystemManager.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>

using namespace std;

string replaceBackslashes(const string &path) {
    string pathCopy = path;
    replace(pathCopy.begin(), pathCopy.end(), '\\', '/');
    return pathCopy;
}

void SystemManager::run(const char *databaseFilePath, const char *cmdScriptFilePath, const char *logFilePath) {

    auto commandFunction = [this](const vector<string> &arguments) {
        commandProcessor.runCommand(arguments);
    };


    Logger &logger = Logger::getInstance(logFilePath, databaseFilePath);
    logger.headerLog(databaseFilePath, cmdScriptFilePath, logFilePath);

    readLines(cmdScriptFilePath, commandFunction);
}

void SystemManager::readLines(const string &filename, const function<void(vector<string> &)> &runCommand) {
    string normalizedPath = replaceBackslashes(filename);
    ifstream input(normalizedPath, ios::binary);  // open in binary mode

    if (!input.is_open()) {
        throw runtime_error("Unable to open file: " + filename);
    }

    string line;
    while (getline(input, line)) {
        istringstream iss(line);
        vector<string> arguments;
        arguments.reserve(6);
        string argument;

        // split line by tab
        while (getline(iss, argument, '\t')) {
            arguments.push_back(argument);
        }

        if (arguments.empty() || arguments[0].empty() || arguments[0][0] == ';') {
            // empty or comment line, skip
            continue;
        }
        // run the command
        runCommand(arguments);
    }
}

void SystemManager::readDatabase(const string &filename, const function<void(string &line)> &processLine) {
    string normalizedPath = replaceBackslashes(filename);
    ifstream file(normalizedPath, ios::binary);  // open in binary mode
    string line;

    while (true) {
        if (!getline(file, line)) {
            break;
        }

        stringstream ss(line);

        processLine(line);
    }
}


void SystemManager::writeLinesToFile(ofstream &file, const vector<string> &lines) {
    if (file.is_open()) {
        copy(lines.begin(), lines.end(), ostream_iterator<string>(file, "\n"));
    } else {
        throw runtime_error("File is not open");
    }
}

streampos SystemManager::writeLineToFile(ofstream &file, const string &line) {
    if (file.is_open()) {
        streampos offset = file.tellp();
        file << line << "\n";
        return offset;
    } else {
        throw runtime_error("File is not open");
    }
}

void SystemManager::createOrTruncateFile(ofstream &file, const string &filename) {
    string normalizedPath = replaceBackslashes(filename);
    file.open(normalizedPath, ios::out | ios::binary);  // open in output mode (create or truncate) and binary mode
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << "\n";
    }
}

void SystemManager::createOrAppendFile(ofstream &file, const string &filename) {
    string normalizedPath = replaceBackslashes(filename);
    file.open(normalizedPath, ios::app | ios::binary);  // open in append mode (create or append) and binary mode
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << "\n";
    }
}

void SystemManager::closeFile(ofstream &file) {
    if (file.is_open()) {
        file.close();
    }
}

Record SystemManager::goToOffset(const string &filename, int offset) {
    ifstream file;

    // Open the file in binary mode
    string normalizedPath = replaceBackslashes(filename);
    file.open(normalizedPath, ios::binary | ios::in);  // add the ios::in flag to open for reading
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << "\n";
        throw runtime_error("Failed to open file: " + filename);
    }

    // Seek to the specified offset
    file.seekg(offset, ios::beg);
    if (!file.good()) {
        cerr << "Failed to seek to offset: " << offset << "\n";
        throw runtime_error("Failed to seek to offset: " + to_string(offset));
    }

    // Move the cursor to the beginning of the line.
    // This corrects the position if the offset was in the middle of a line.

    if (offset != 0) {
        file.seekg(-1, ios::cur);  // move one character back
        char c;
        while (file.get(c)) {
            if (c == '\n') {  // we've hit the previous line ending
                break;  // stop moving back
            }
            file.seekg(-2, ios::cur);  // move another character back
        }
    }

    // Read the line
    string row;
    getline(file, row);

    // Close the file
    file.close();
    return {offset, row};
}
