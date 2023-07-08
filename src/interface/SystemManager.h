#ifndef INC_8042_PROJECT_SYSTEMMANAGER_H
#define INC_8042_PROJECT_SYSTEMMANAGER_H

#include "CommandProcessor.h"
#include <functional>
#include <fstream>

class SystemManager {
public:
    // Public static member function to access the instance
    static SystemManager &getInstance() {
        // Create the instance on the first call
        // Return the existing instance on subsequent calls
        static SystemManager instance;
        return instance;
    }

    CommandProcessor commandProcessor;

    SystemManager() = default;

    // Delete the copy constructor and assignment operator to prevent copies
    SystemManager(const SystemManager &) = delete;

    SystemManager &operator=(const SystemManager &) = delete;

    static void
    readLines(const std::string &filename, const std::function<void(std::vector<std::string> &)> &runCommand);

    static void readDatabase(const std::string &filename, const std::function<void(std::string &line)> &processLine);

    void run(const char *databaseFilePath, const char *cmdScriptFilePath, const char *logFilePath);

    static std::streampos writeLineToFile(std::ofstream &file, const std::string &line);

    static void writeLinesToFile(std::ofstream &file, const std::vector<std::string> &lines);

    static void createOrTruncateFile(std::ofstream &file, const std::string &filename);

    static void createOrAppendFile(std::ofstream &file, const std::string &filename);

    static void closeFile(std::ofstream &file);

    static Record goToOffset(const std::string &filename, int offset);
};


#endif //INC_8042_PROJECT_SYSTEMMANAGER_H
