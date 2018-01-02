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


/* Returns a map of string key value pairs, representing the settings read in from
 * the file with the passed name.
 * This is raw information which will need to be parsed by specialised classes.
 */
std::map<std::string, std::string> FileIO::loadSettings(const std::string& _file_name)
{
    std::string path("Resources/" + _file_name);
    std::map<std::string, std::string> settings;

    // Open the settings file, or create a default one if it can't be found.
    std::ifstream file(path);
    if (!file.is_open())
    {
        FileIO::createDefaultSettingsFile(_file_name);
        file.open(path);
    }

    // Process the settings file.
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


/* Creates a settings file with default values, if a layout has been hardcoded
 * for a file with the passed name.
 */
void FileIO::createDefaultSettingsFile(const std::string& _file_name)
{
    if (_file_name == "SimulationSettings.txt")
    {
        std::ofstream file("Resources/SimulationSettings.txt");

        file
            << "level=level2.txt\n"
            << "max_agents=50000\n"
            << "agents_per_spawn=100\n"
            << "agent_speed=25\n"
            << "agent_steer=1\n"
            << "agent_separation=1\n";

        file.close();
    }
    else
    {
        throw std::runtime_error("No default settings provided for " + _file_name);
    }
}
