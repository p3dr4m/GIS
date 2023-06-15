#ifndef INC_8042_PROJECT_LOGGER_H
#define INC_8042_PROJECT_LOGGER_H

#include <string>
#include <vector>
#include <fstream>
#include "SystemManager.h"

class Logger {
public:
    static Logger &getInstance(const std::string &logFilePath = "", const std::string &databaseFilePath = "") {
        static Logger instance(logFilePath, databaseFilePath);
        return instance;
    }


    void headerLog(const std::string &databaseFilePath, const std::string &cmdScriptFilePath,
                   const std::string &logFilePath);

    void importLog(const std::vector<std::string> &arguments, std::vector<int> data);

    void worldLog(const std::vector<std::string> &arguments);

    void debugWorld(const std::vector<std::string> &option, PRQuadTree &tree);
    void debugQuad(const std::vector<std::string> &option, PRQuadTree &tree);

    void whatIsInLog(std::vector<std::string> arguments, std::vector<int> records);
    void recurseTree(PRQuadTree &tree, std::vector<std::vector<int>> &grid, int minX, int maxX, int minY, int maxY);
private:

    Logger(const std::string &logFilePath, const std::string &databaseFilePath) {
        // create or truncate log file and database file
        SystemManager::createOrTruncateFile(logFile, logFilePath);
        SystemManager::createOrTruncateFile(databaseFile, databaseFilePath);
        SystemManager::closeFile(logFile);
        SystemManager::closeFile(databaseFile);
        SystemManager::createOrAppendFile(logFile, logFilePath);
        SystemManager::createOrAppendFile(databaseFile, databaseFilePath);
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
        if (databaseFile.is_open()) {
            databaseFile.close();
        }
    }

    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;
    int cmdCount = 0;
    std::string separator = "------------------------------------------------------------------------------------------";
    std::ofstream logFile;       // File stream for log.

    std::ofstream databaseFile;  // File stream for database.

    static std::string getTime();

    void printWorld(PRQuadTree &tree, std::vector<std::string> &lines);
};


#endif //INC_8042_PROJECT_LOGGER_H
