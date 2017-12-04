#pragma once

#include "DXMathHelper.h"
#include "ListenerSubject.h"
#include "AgentListener.h"

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
    SwarmAgent(AgentInstanceData& _data);
    ~SwarmAgent() = default;

    void tick(GameData* _gd);

    void steerFromNeighbourAgents(std::vector<SwarmAgent*> _neighbours);
    void applySteer(const DirectX::XMFLOAT3& _force);

    const DirectX::XMFLOAT3& getPos() const;
    void setPos(const DirectX::XMFLOAT3& _pos);
    void setPos(const float _x, const float _y, const float _z);
    void adjustPos(const DirectX::XMFLOAT3& _adjustment);
    void adjustPos(const float _x, const float _y, const float _z);

    const DirectX::XMFLOAT4& getCol() const;
    void setColor(const DirectX::XMFLOAT4& _color);
    void setColor(const float _r, const float _g, const float _b, const float _a);

    int getCurrentTileIndex() const;
    void setCurrentTileIndex(const int _tile_index);

private:
    void move(GameData* _gd);

    AgentInstanceData& instance_data;

    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT3 acceleration;

    int current_tile_index;

};
