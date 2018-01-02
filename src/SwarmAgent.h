#pragma once

#include "DXMathHelper.h"
#include "ListenerSubject.h"
#include "AgentListener.h"
#include "AgentSettings.h"

struct GameData;

/* Contains information about the agent which translates directly onto the GPU-side for rendering.
 * The layout of this data must match the input layout of the shader that the agent model uses.
 */
struct AgentInstanceData
{
    AgentInstanceData()
        : pos(0, 0, 0)
        , color(1, 1, 1, 1)
    {
    }

    DirectX::XMFLOAT3 pos;      // World space position of the agent.
    DirectX::XMFLOAT4 color;    // Colour of the agent.
};

class SwarmAgent : public ListenerSubject<AgentListener>
{
public:
    SwarmAgent(AgentInstanceData& _data, AgentSettings& _settings);
    ~SwarmAgent() = default;

    void tick(const float _dt);

    void steerFromNeighbourAgents(const std::vector<SwarmAgent*>& _neighbours);
    void applySteer(const DirectX::XMFLOAT3& _force);

    const DirectX::XMFLOAT3& getPos() const;
    void setPos(const DirectX::XMFLOAT3& _pos);
    void setPos(const float _x, const float _y, const float _z);

    const DirectX::XMFLOAT4& getCol() const;
    void setColor(const DirectX::XMFLOAT4& _color);
    void setColor(const float _r, const float _g, const float _b, const float _a);

    int getCurrentTileIndex() const;
    void setCurrentTileIndex(const int _tile_index);

private:
    void move(const float _dt);

    AgentInstanceData& instance_data;   // Information used by the GPU and CPU.
    AgentSettings& settings;            // Agent characteristics.

    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT3 acceleration;

    int current_tile_index;             // E.g. which NavNode bin this agent is in.

};
