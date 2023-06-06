#ifndef INC_8042_PROJECT_COMMANDPROCESSOR_H
#define INC_8042_PROJECT_COMMANDPROCESSOR_H

#include "GISRecord.h"

enum Command {
    world, import, debug, what_is_at, what_is, what_is_in, quit
};

class CommandProcessor {
private:
    void worldCmd(std::vector<std::string> arguments);

    void importCmd(std::vector<std::string> arguments);

    void debugCmd(std::vector<std::string> arguments);

    void whatIsAtCmd(std::vector<std::string> arguments);

    void whatIsCmd(std::vector<std::string> arguments);

    void whatIsInCmd(std::vector<std::string> arguments);

    static void quitCmd();

    GISRecord gisRecord;

public:
    void parseCmdScript(std::vector<std::string> arguments);


};


#endif //INC_8042_PROJECT_COMMANDPROCESSOR_H
