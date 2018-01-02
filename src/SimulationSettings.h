#pragma once

#include <string>

#include "AgentSettings.h"

/* Contains information parsed from the SimulationSettings.txt file.
 * The settings file is parsed internally, therefore the data can only be retrieved.
 */
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
