

#ifndef INC_8042_PROJECT_SYSTEMMANAGER_H
#define INC_8042_PROJECT_SYSTEMMANAGER_H

#include <string>
#include <vector>
#include "CommandProcessor.h"

class SystemManager {
public:
    // Public static member function to access the instance
    static SystemManager &getInstance() {
        // Create the instance on the first call
        // Return the existing instance on subsequent calls
        static SystemManager instance;
        return instance;
    }

    // Delete the copy constructor and assignment operator to prevent copies
    SystemManager(const SystemManager &) = delete;

    SystemManager &operator=(const SystemManager &) = delete;
    Logger logger;
    CommandProcessor commandProcessor;

    DMS fillDMS(std::string value);

    void readScript(std::string &filename);

private:
    // Private constructor to prevent direct instantiation
    SystemManager() {}
};


#endif //INC_8042_PROJECT_SYSTEMMANAGER_H
