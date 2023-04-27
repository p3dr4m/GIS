//
// Created by Raziq on 2023-04-26.
//

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include "CommandProcessor.h"


int main(int argc, char *argv[]) {
    std::string filename = argv[1];

    // create command processor
    CommandProcessor commandProcessor;
    commandProcessor.readFile(filename);


}
    