#include <fstream>
#include <iostream>

#include "FileIO.h"


Level FileIO::loadLevel(const std::string& _file_name)
{
    Level level;

    std::ifstream file("Resources/Levels/" + _file_name);
    if (!file.is_open())
    {
        std::cout << "Unable to find level file: " << _file_name.c_str() << std::endl;
        return level;
    }

    file >> level.width >> level.height;

    level.data.reserve(level.width * level.height);
    char elem = 0;

    for (int row = 0; row < level.height; ++row)
    {
        for (int col = 0; col < level.width; ++col)
        {
            file >> elem;
            level.data.push_back(elem);
        }
    }

    return level;
}
