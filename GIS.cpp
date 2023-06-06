#include <iostream>
#include "src/SystemManager.h"
#include "src/CommandProcessor.h"

using namespace std;

/**
 * •/GIS <database file name> <command script file name> ‹log file name>
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Invalid number of arguments" << std::endl;
        // explain how to use the command
        std::cerr << "Usage: GIS <database file name> <command script file name> <log file name>" << std::endl;
        // exit with error
        exit(1);
    }
    char *databaseFilePath = argv[1];
    char *cmdScriptFilePath = argv[2];
    char *logFilePath = argv[3];

    std::cout << "database file name: " << databaseFilePath << std::endl;
    std::cout << "command script file name: " << cmdScriptFilePath << std::endl;
    std::cout << "log file name: " << logFilePath << std::endl;

    CommandProcessor commandProcessor;
    commandProcessor.parseCmdScript(cmdScriptFilePath);
    return 0;
}
