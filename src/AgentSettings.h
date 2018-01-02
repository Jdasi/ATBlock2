#pragma once

#include "DXMathHelper.h"

/* Encapuslates the capabilities and appearance of a SwarmAgent.
 */
struct AgentSettings
{
    AgentSettings()
        : max_agents(0)
        , agents_per_spawn(0)
        , agent_speed(0)
        , agent_steer(0)
        , agent_separation(0)
        , agent_speed_sqr(0)
        , agent_steer_sqr(0)
        , agent_separation_sqr(0)
        , color(0, 0, 0, 1)
    {
    }

    unsigned int max_agents;
    unsigned int agents_per_spawn;

    float agent_speed;
    float agent_steer;
    float agent_separation;

    float agent_speed_sqr;
    float agent_steer_sqr;
    float agent_separation_sqr;

    DirectX::XMFLOAT4 color;

};
