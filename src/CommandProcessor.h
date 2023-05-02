//
// Created by Pedram Nazari on 2023-04-28.
//

#ifndef INC_8042_PROJECT_COMMANDPROCESSOR_H
#define INC_8042_PROJECT_COMMANDPROCESSOR_H

#include <string>
#include <vector>

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

public:
    void parseCmdScript(const std::string& filename);

};


#endif //INC_8042_PROJECT_COMMANDPROCESSOR_H
