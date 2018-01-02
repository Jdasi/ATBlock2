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


std::map<std::string, std::string> FileIO::loadSettings(const std::string& _file_name)
{
    std::string path("Resources/" + _file_name);
    std::map<std::string, std::string> settings;

    std::ifstream file(path);
    if (!file.is_open())
    {
        FileIO::createDefaultSettingsFile();
        file.open(path);
    }

    std::string line;
    while (std::getline(file, line))
    {
        int pos = line.find_first_of('=');

        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);

        settings[key] = val;
    }

    return settings;
}


void FileIO::createDefaultSettingsFile()
{
    std::ofstream file("Resources/SimulationSettings.txt");

    file
        << "level = level2.txt"
        << "max_agents=50000"
        << "agents_per_spawn=100"
        << "agent_speed=25"
        << "agent_steer=1"
        << "agent_separation=1";

    file.close();
}
