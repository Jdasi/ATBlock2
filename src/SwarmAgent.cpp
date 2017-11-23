#include "SwarmAgent.h"
#include "GameData.h"
#include "JTime.h"


SwarmAgent::SwarmAgent()
    : pos(0, 0, 0)
    , color(1, 1, 1, 1)
    , velocity(0, 0, 0)
    , acceleration(0, 0, 0)
    , current_tile_index(0)
    , max_speed(20)
    , max_steer(1)
    , max_speed_sqr(max_speed * max_speed)
    , max_steer_sqr(max_steer * max_steer)
{
}


void SwarmAgent::tick(GameData* _gd)
{
    // Generate forces.
    //applyForce(generateSeekForce());

    // Apply them.
    move(_gd);

    // Reset for next tick.
    acceleration.x = 0;
    acceleration.y = 0;
    acceleration.z = 0;
}


const DirectX::XMFLOAT3& SwarmAgent::getPos() const
{
    return pos;
}


void SwarmAgent::setPos(const DirectX::XMFLOAT3& _pos)
{
    pos = _pos;
}


void SwarmAgent::setPos(const float _x, const float _y, const float _z)
{
    setPos(DirectX::XMFLOAT3(_x, _y, _z));
}


void SwarmAgent::adjustPos(const DirectX::XMFLOAT3& _adjustment)
{
    adjustPos(_adjustment.x, _adjustment.y, _adjustment.z);
}


void SwarmAgent::adjustPos(const float _x, const float _y, const float _z)
{
    pos.x += _x;
    pos.y += _y;
    pos.z += _z;
}


const DirectX::XMFLOAT4& SwarmAgent::getCol() const
{
    return color;
}


void SwarmAgent::setColor(const DirectX::XMFLOAT4& _color)
{
    color = _color;
}


void SwarmAgent::setColor(const float _r, const float _g, const float _b, const float _a)
{
    setColor(DirectX::XMFLOAT4(_r, _g, _b, _a));
}


int SwarmAgent::getCurrentTileIndex() const
{
    return current_tile_index;
}


void SwarmAgent::setCurrentTileIndex(const int _tile_index)
{
    if (current_tile_index != _tile_index)
    {
        // TODO: inform spatial partioning system of tile change ..
    }

    current_tile_index = _tile_index;
}


void SwarmAgent::applySteer(const DirectX::XMFLOAT3& _force)
{
    DirectX::XMFLOAT3 desired = DirectX::Float3Mul(_force, max_speed);
    DirectX::XMFLOAT3 change = DirectX::Float3SubtractBfromA(desired, velocity);
    change = DirectX::Float3Mul(change, max_steer / max_speed);

    acceleration = DirectX::Float3Add(acceleration, change);
}


void SwarmAgent::move(GameData* _gd)
{
    velocity = DirectX::Float3Add(velocity, acceleration);

    // Limit speed.
    if (DirectX::Float3MagnitudeSquared(velocity) > max_speed_sqr)
    {
        velocity = DirectX::Float3Normalized(velocity);
        velocity = DirectX::Float3Mul(velocity, max_speed);
    }

    DirectX::XMFLOAT3 step = DirectX::Float3Mul(velocity, JTime::getDeltaTime());
    pos = DirectX::Float3Add(pos, step);
}
