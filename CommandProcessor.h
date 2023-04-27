//
// Created by Raziq on 2023-04-26.
//

#ifndef FINALPROJALGO_COMMANDPROCESSOR_H
#define FINALPROJALGO_COMMANDPROCESSOR_H

#include <string>

enum Command {world, import, debug, what_is_at, what_is, what_is_in, quit};

class CommandProcessor {
private:
    Command world;
    Command import;
    Command debug;
    Command what_is_at;
    Command what_is;
    Command what_is_in;
    Command quit;

public:
    void SetCommand(Command world, Command import, Command debug,Command what_is_at, Command what_is, Command what_is_in, Command quit);
    void readFile(std::string filename);
};



#endif //FINALPROJALGO_COMMANDPROCESSOR_H
