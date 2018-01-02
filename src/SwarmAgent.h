#pragma once

#include "DXMathHelper.h"
#include "ListenerSubject.h"
#include "AgentListener.h"
#include "AgentSettings.h"

struct GameData;

struct AgentInstanceData
{
    AgentInstanceData()
        : pos(0, 0, 0)
        , color(1, 1, 1, 1)
    {
    }

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;
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

    AgentInstanceData& instance_data;
    AgentSettings& settings;

    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT3 acceleration;

    int current_tile_index;

};
