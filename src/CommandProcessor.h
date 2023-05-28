#ifndef INC_8042_PROJECT_COMMANDPROCESSOR_H
#define INC_8042_PROJECT_COMMANDPROCESSOR_H

#include "GISRecord.h"
#include "SystemManager.h"

enum Command {
    world, import, debug, what_is_at, what_is, what_is_in, quit
};

class CommandProcessor {
private:
    void worldCmd(vector<string> arguments);

    void importCmd(vector<string> arguments);

    void debugCmd(vector<string> arguments);

    void whatIsAtCmd(vector<string> arguments);

    void whatIsCmd(vector<string> arguments);

    void whatIsInCmd(vector<string> arguments);

    static void quitCmd();

    GISRecord gisRecord;
    SystemManager systemManager;

public:
    void parseCmdScript(const string &filename);


};


#endif //INC_8042_PROJECT_COMMANDPROCESSOR_H
