#include "SimulationSettings.h"
#include "FileIO.h"


SimulationSettings::SimulationSettings()
    : level_name()
{
    auto settings = FileIO::loadSettings("SimulationSettings.txt");
    
    for (auto& elem : settings)
    {
        parseSetting(elem.first, elem.second);
    }

    agent_settings.color = DirectX::XMFLOAT4(1, 0, 0, 1);
}


const std::string& SimulationSettings::getLevelName() const
{
    return level_name;
}


const unsigned int SimulationSettings::getMaxAgents() const
{
    return agent_settings.max_agents;
}


const unsigned int SimulationSettings::getAgentsPerSpawn() const
{
    return agent_settings.agents_per_spawn;
}


const float SimulationSettings::getAgentSpeed() const
{
    return agent_settings.agent_speed;
}


const float SimulationSettings::getAgentSteer() const
{
    return agent_settings.agent_steer;
}


const float SimulationSettings::getAgentSeparation() const
{
    return agent_settings.agent_separation;
}


AgentSettings& SimulationSettings::getAgentSettings()
{
    return agent_settings;
}


void SimulationSettings::parseSetting(const std::string& _key, const std::string& _value)
{
    if (_key == "level")
    {
        level_name = _value;
    }
    else if (_key == "max_agents")
    {
        agent_settings.max_agents = atoi(_value.c_str());
    }
    else if (_key == "agents_per_spawn")
    {
        agent_settings.agents_per_spawn = atoi(_value.c_str());
    }
    else if (_key == "agent_speed")
    {
        float val = static_cast<float>(atof(_value.c_str()));

        agent_settings.agent_speed = val;
        agent_settings.agent_speed_sqr = val * val;
    }
    else if (_key == "agent_steer")
    {
        float val = static_cast<float>(atof(_value.c_str()));

        agent_settings.agent_steer = val;
        agent_settings.agent_steer_sqr = val * val;
    }
    else if (_key == "agent_separation")
    {
        float val = static_cast<float>(atof(_value.c_str()));

        agent_settings.agent_separation = val;
        agent_settings.agent_separation_sqr = val * val;
    }
}
