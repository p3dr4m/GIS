//
// Created by Raziq on 2023-04-26.
//

#include "CommandProcessor.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>

void CommandProcessor::readFile(std::string filename) {

    // read file using filename
    std::ifstream file(filename);
    std::string line;

    // if file is open
    if (file.is_open()) {
        // while there is a line
        while (getline(file, line)) {
            if (line[0] == ';') {
                continue;
            }
            // print line
            std::cout << line << std::endl;
        }
        // close file
        file.close();
    }

}

void CommandProcessor::SetCommand(Command world, Command import, Command debug, Command what_is_at, Command what_is,
                                  Command what_is_in, Command quit) {
    CommandProcessor::world = world;
    CommandProcessor::import = import;
    CommandProcessor::debug = debug;
    CommandProcessor::what_is_at = what_is_at;
    CommandProcessor::what_is = what_is;
    CommandProcessor::what_is_in = what_is_in;
    CommandProcessor::quit = quit;


}