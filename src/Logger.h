//
// Created by Raziq on 2023-04-26.
//

#ifndef INC_8042_PROJECT_LOGGER_H
#define INC_8042_PROJECT_LOGGER_H

#pragma once

#include <string>

class Logger {
public:
    enum LogType {
        INFO,
        WARNING,
        ERROR,
        END,
        WORLD,
        IMPORT,
        DEBUG,
        WHAT_IS,
        WHAT_IS_AT,
        WHAT_IS_IN
    };

    std::string logFileName;

    void log(std::string msg);

    void log(std::string message, LogType type, bool sepBegin, bool sepEnd);

    void writeToLog(std::string line, std::string filename);

    std::string separator = "\n------------------------------------------------------------------------------------------\n";
private:
    int cmdNum = 0;
    std::string worldLog =
            "Latitude / longitude values in index entries are shown as floating point values, in decimal long/lat format." +
            separator + "\t\t\t\t\t\tWorld boundaries are set to:\n";
    std::string initialLog = "Course Project for COMP 8042\nStudent Name : Kevin Vilanova, Student Id : A01019400\nBegin of GIS Program log : \n";


};


#endif //INC_8042_PROJECT_LOGGER_H
