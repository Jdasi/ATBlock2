#pragma once

class SwarmAgent;

class AgentListener
{
public:
    AgentListener() = default;
    ~AgentListener() = default;

    virtual void onTileIndexChanged(SwarmAgent* _agent, const int _prev_index,
        const int _new_index) {}

};
