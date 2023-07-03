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

    void debugPool(BufferPool<Record> bufferPool);

    void whatIsInLog(std::vector<std::string> arguments, std::vector<Record> records);

    void whatIsAtLog(std::vector<std::string> arguments, std::vector<Record> records, std::vector<int> offsets);

    void whatIsLog(std::vector<std::string> arguments, std::vector<Record> records, std::vector<int> offsets);

    void recurseTree(PRQuadTree &tree, std::vector<std::vector<int>> &grid, int minX, int maxX, int minY, int maxY);

    void printDebugQuad(std::vector<std::string> &lines, PRQuadTree &tree, int depth);

    void debugHash(const std::string& hashTableStr);

    void quitCmd(std::vector<std::string> arguments);

    int logToDatabase(const std::string &line, int offset);

    void openDbFile() {
        if (!databaseFile.is_open()) {
            databaseFile.open(databaseFilePath, std::ios::app);
        }
    }

    void openLogFile() {
        if (!logFile.is_open()) {
            logFile.open(logFilePath, std::ios::app);
        }
    }

    void closeDbFile() {
        if (databaseFile.is_open()) {
            databaseFile.close();
        }
    }

    void closeLogFile() {
        if (logFile.is_open()) {
            logFile.close();
        }
    };

    void log(const std::string &line) {
        databaseFile << line << std::endl;
    }

    std::string getLogFilePath() const {
        return logFilePath;
    }

    std::string getDatabaseFilePath() const {
        return databaseFilePath;
    }

private:
    Logger(const std::string &logFilePath,
           const std::string &databaseFilePath) :
            logFilePath(logFilePath),
            databaseFilePath(databaseFilePath) {
        // create or truncate log file and database file
        SystemManager::createOrTruncateFile(logFile, logFilePath);
        SystemManager::createOrTruncateFile(databaseFile, databaseFilePath);
        SystemManager::closeFile(logFile);
        SystemManager::closeFile(databaseFile);
        SystemManager::createOrAppendFile(logFile, logFilePath);
        SystemManager::createOrAppendFile(databaseFile, databaseFilePath);
    }

    ~Logger() {
        closeDbFile();
        closeLogFile();
    }

    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

    int cmdCount = 0;
    std::string separator = "------------------------------------------------------------------------------------------";
    std::ofstream logFile;       // File stream for log.
    std::ofstream databaseFile;  // File stream for database.
    std::string logFilePath;
    std::string databaseFilePath;

    static std::string getTime();

    void printWorld(PRQuadTree &tree, std::vector<std::string> &lines);


};


#endif //INC_8042_PROJECT_LOGGER_H