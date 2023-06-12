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
        cerr << "Invalid number of arguments" << endl;
        // explain how to use the command
        cerr << "Usage: GIS <database file name> <command script file name> <log file name>" << endl;
        // exit with error
        exit(1);
    }
    char *databaseFilePath = argv[1];
    char *cmdScriptFilePath = argv[2];
    char *logFilePath = argv[3];

    cout << "database file name: " << databaseFilePath << endl;
    cout << "command script file name: " << cmdScriptFilePath << endl;
    cout << "log file name: " << logFilePath << endl;

    CommandProcessor commandProcessor;
    commandProcessor.parseCmdScript(cmdScriptFilePath);
    return 0;
}
