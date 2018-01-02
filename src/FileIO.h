#pragma once

#include <map>
#include <string>

#include "Level.h"

// Utility/helper functions to do with file input/output should go here.

namespace FileIO
{
    Level loadLevel(const std::string& _file_name);
    std::map<std::string, std::string> loadSettings(const std::string& _file_name);

    void createDefaultSettingsFile(const std::string& _file_name);
}
