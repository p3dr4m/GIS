#ifndef INC_8042_PROJECT_SYSTEMMANAGER_H
#define INC_8042_PROJECT_SYSTEMMANAGER_H

#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include "CommandProcessor.h"

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

    static std::fpos<mbstate_t> writeLineToFile(std::ofstream &file, const std::string &line);

    static void writeLinesToFile(std::ofstream &file, const std::vector<std::string> &lines);

    // Method to create a file or truncate it if it exists.
    static void createOrTruncateFile(std::ofstream &file, const std::string &filename);

    // Method to create a file or open it for appending if it exists.
    static void createOrAppendFile(std::ofstream &file, const std::string &filename);

    // Don't forget to close the file when you're done with it.
    static void closeFile(std::ofstream &file);

    static Record goToOffset(std::ifstream &file, const std::string &filename, int offset);

    static std::string replaceBackslashes(const std::string &path);
};


#endif //INC_8042_PROJECT_SYSTEMMANAGER_H
