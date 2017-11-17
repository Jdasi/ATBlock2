#pragma once

#include "DXMathHelper.h"

struct GameData;

class SwarmAgent
{
public:
    SwarmAgent();
    ~SwarmAgent() = default;

    void tick(GameData* _gd);

    void setSeekPos(const DirectX::XMFLOAT3& _pos);

    const DirectX::XMFLOAT3& getPos() const;
    void setPos(const DirectX::XMFLOAT3& _pos);
    void setPos(const float _x, const float _y, const float _z);
    void adjustPos(const float _x, const float _y, const float _z);

    const DirectX::XMFLOAT4& getCol() const;
    void setColor(const DirectX::XMFLOAT4& _color);
    void setColor(const float _r, const float _g, const float _b, const float _a);

private:
    DirectX::XMFLOAT3 generateSeekForce();
    DirectX::XMFLOAT3 generateAvoidanceForce();
    void applyForce(const DirectX::XMFLOAT3& _force);
    void move(GameData* _gd);

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;

    DirectX::XMFLOAT3 seek_pos;
    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT3 acceleration;

    // Settings.
    float max_speed;
    float max_steer;
    float max_speed_sqr; // Squared value.
    float max_steer_sqr; // Squared value.

};
