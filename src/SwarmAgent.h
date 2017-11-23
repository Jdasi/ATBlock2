#pragma once

#include "DXMathHelper.h"

struct GameData;

class SwarmAgent
{
public:
    SwarmAgent();
    ~SwarmAgent() = default;

    void tick(GameData* _gd);

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

    // Variable ordering must match instance shader input layout.
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;

    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT3 acceleration;

    int current_tile_index;

    // Settings.
    float max_speed;
    float max_steer;
    float max_speed_sqr; // Squared value.
    float max_steer_sqr; // Squared value.

};
