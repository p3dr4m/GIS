#include "SystemManager.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>

using namespace std;


void SystemManager::run(const char *databaseFilePath, const char *cmdScriptFilePath, const char *logFilePath) {

    auto commandFunction = [this](const vector<string> &arguments) {
        commandProcessor.runCommand(arguments);
    };


    Logger &logger = Logger::getInstance(logFilePath, databaseFilePath);
    logger.headerLog(databaseFilePath, cmdScriptFilePath, logFilePath);

    readLines(cmdScriptFilePath, commandFunction);
}

void SystemManager::readLines(const string &filename, const function<void(vector<string> &)> &runCommand) {
    ifstream input(filename);

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

void SystemManager::readDatabase(const string &filename, const function<void(vector<string> &)> &processLine) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        vector<string> words;
        string word;
        stringstream ss(line);

        while (getline(ss, word, '|')) {
            words.push_back(word);
        }

        // Add file offset as the last element
        words.push_back(to_string(file.tellg()));

        processLine(words);
    }
}


void SystemManager::writeLinesToFile(ofstream &file, const vector<string> &lines) {
    if (file.is_open()) {
        copy(lines.begin(), lines.end(), ostream_iterator<string>(file, "\n"));
    } else {
        throw runtime_error("File is not open");
    }
}

void SystemManager::writeLineToFile(ofstream &file, const string &line) {
    if (file.is_open()) {
        file << line << "\n";
    } else {
        throw runtime_error("File is not open");
    }
}

void SystemManager::createOrTruncateFile(ofstream &file, const string &filename) {
    file.open(filename, ios::out);  // open in output mode (create or truncate)
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
    }
}

void SystemManager::createOrAppendFile(ofstream &file, const string &filename) {
    file.open(filename, ios::app);  // open in append mode (create or append)
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
    }
}

void SystemManager::closeFile(ofstream &file) {
    if (file.is_open()) {
        file.close();
    }
}

Record SystemManager::goToOffset(ifstream &file, const string &filename, int offset) {
    // Open the file in binary mode
    file.open(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // Seek to the specified offset
    file.seekg(offset, ios::beg);
    if(!file.good()) {
        cerr << "Failed to seek to offset: " << offset << endl;
        throw std::runtime_error("Failed to seek to offset: " + std::to_string(offset));
    }

    // Read the line
    string row;
    getline(file, row);

    // Close the file
    file.close();

    return {offset, row};
}
