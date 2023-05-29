#include <iostream>
#include <fstream>
#include "Logger.h"

using namespace std;

void Logger::log(std::string message) {
    writeToLog(message, logFileName);
}

void Logger::log(std::string message, LogType type, bool sepBegin, bool sepEnd) {
    std::string log;

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
            log += std::to_string(cmdNum) + ": " + message + separator;
            break;
        case IMPORT:
            cmdNum++;
            log += "Command " + std::to_string(cmdNum) + ": " + message;
            break;
        case DEBUG:
            cmdNum++;
            log += "Command " + std::to_string(cmdNum) + ": " + message;
            break;
        case WHAT_IS:
            cmdNum++;
            log += "Command " + std::to_string(cmdNum) + ": " + message;
            break;
        case WHAT_IS_AT:
            cmdNum++;
            log += "Command " + std::to_string(cmdNum) + ": " + message;
            break;
        case WHAT_IS_IN:
            cmdNum++;
            log += "Command " + std::to_string(cmdNum) + ": " + message;
            break;
    }

    if (sepEnd)
        log += separator;

    // write to log
    writeToLog(log, logFileName);

}

//Export one line to a file.
void Logger::writeToLog(std::string line, std::string filename) {
    // output file
    std::ofstream out;
    out.open(filename, std::ios_base::app);

    out << line << "\n";
    out.close();
}