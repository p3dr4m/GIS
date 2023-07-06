#ifndef INC_8042_PROJECT_COMMANDPROCESSOR_H
#define INC_8042_PROJECT_COMMANDPROCESSOR_H

#include "../GISRecord.h"


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


    void quitCmd(std::vector<std::string> arguments);

    GISRecord gisRecord;
public:


    void runCommand(const std::vector<std::string> &commands);

};


#endif //INC_8042_PROJECT_COMMANDPROCESSOR_H
