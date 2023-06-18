#include <iostream>
#include "src/interface/CommandProcessor.h"
#include "src/interface/SystemManager.h"
using namespace std;

/**
 * •/GIS <database file name> <command script file name> ‹log file name>
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        throw invalid_argument("Invalid number of arguments\n"
                               "Usage: GIS <database file name> <command script file name> <log file name>");
    }
    char *databaseFilePath = argv[1];
    char *cmdScriptFilePath = argv[2];
    char *logFilePath = argv[3];

    SystemManager systemManager;
    systemManager.run(databaseFilePath, cmdScriptFilePath, logFilePath);
    return 0;
}
