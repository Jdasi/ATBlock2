#pragma once

#include <vector>

#include "Level.h"

namespace FileIO
{
    Level loadLevel(const std::string& _file_name);
}
