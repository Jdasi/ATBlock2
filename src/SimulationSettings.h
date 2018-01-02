#pragma once

#include <string>

#include "AgentSettings.h"

class SimulationSettings
{
public:
    SimulationSettings();
    ~SimulationSettings() = default;

    const std::string& getLevelName() const;
    
    const unsigned int getMaxAgents() const;
    const unsigned int getAgentsPerSpawn() const;

    const float getAgentSpeed() const;
    const float getAgentSteer() const;
    const float getAgentSeparation() const;

    AgentSettings& getAgentSettings();

private:
    void parseSetting(const std::string& _key, const std::string& _value);

    std::string level_name;
    AgentSettings agent_settings;

};
