#include <iostream>
#include <fstream>
#include "Logger.h"

using namespace std;

void Logger::log(string message) {
    writeToLog(message, logFileName);
}

void Logger::log(string message, LogType type, bool sepBegin, bool sepEnd) {
    string log;

    if (sepBegin)
        log += separator;

    switch (type) {
        case INFO:
            log += "INFO: " + message;
            break;
        case WARNING:
            log += "WARNING: " + message;
            break;
        case ERROR:
            log += "ERROR: " + message;
            break;
        case WORLD:
            log += worldLog + message;
            break;
        case END:
            cmdNum++;
            log += to_string(cmdNum) + ": " + message + separator;
            break;
        case IMPORT:
            cmdNum++;
            log += "Command " + to_string(cmdNum) + ": " + message;
            break;
        case DEBUG:
            cmdNum++;
            log += "Command " + to_string(cmdNum) + ": " + message;
            break;
        case WHAT_IS:
            cmdNum++;
            log += "Command " + to_string(cmdNum) + ": " + message;
            break;
        case WHAT_IS_AT:
            cmdNum++;
            log += "Command " + to_string(cmdNum) + ": " + message;
            break;
        case WHAT_IS_IN:
            cmdNum++;
            log += "Command " + to_string(cmdNum) + ": " + message;
            break;
    }

    if (sepEnd)
        log += separator;

    // write to log
    writeToLog(log, logFileName);

}

//Export one line to a file.
void Logger::writeToLog(string line, string filename) {
    // output file
    ofstream out;
    out.open(filename, ios_base::app);

    out << line << "\n";
    out.close();
}