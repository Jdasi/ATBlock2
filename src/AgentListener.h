#pragma once

class SwarmAgent;

/* Utility class for other classes to inherit from if they wish to listen and
 * react to SwarmAgent behaviour.
 */
class AgentListener
{
public:
    AgentListener() = default;
    ~AgentListener() = default;

    // Called whenever a SwarmAgent moves to a different grid tile.
    virtual void onTileIndexChanged(SwarmAgent* _agent, const int _prev_index,
        const int _new_index) {}

};
