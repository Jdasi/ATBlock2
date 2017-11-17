#include "SwarmAgent.h"
#include "GameData.h"
#include "JTime.h"


SwarmAgent::SwarmAgent()
    : pos(0, 0, 0)
    , color(1, 1, 1, 1)
    , seek_pos(0, 0, 0)
    , velocity(0, 0, 0)
    , acceleration(0, 0, 0)
    , max_speed(20)
    , max_steer(1)
    , max_speed_sqr(max_speed * max_speed)
    , max_steer_sqr(max_steer * max_steer)
{
}


void SwarmAgent::tick(GameData* _gd)
{
    // Generate forces.
    applyForce(generateSeekForce());

    // Apply them.
    move(_gd);

    // Reset for next tick.
    acceleration.x = 0;
    acceleration.y = 0;
    acceleration.z = 0;
}


void SwarmAgent::setSeekPos(const DirectX::XMFLOAT3& _pos)
{
    seek_pos = _pos;
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


DirectX::XMFLOAT3 SwarmAgent::generateSeekForce()
{
    DirectX::XMFLOAT3 desired = DirectX::Float3SubtractBfromA(seek_pos, pos);
    desired = DirectX::Float3Normalized(desired);
    desired = DirectX::Float3Mul(desired, max_speed);

    DirectX::XMFLOAT3 steer = DirectX::Float3SubtractBfromA(desired, velocity);
    if (DirectX::Float3MagnitudeSquared(steer) > max_steer_sqr)
    {
        steer = DirectX::Float3Normalized(steer);
        steer = DirectX::Float3Mul(steer, max_steer);
    }

    return steer;
}


DirectX::XMFLOAT3 SwarmAgent::generateAvoidanceForce()
{
    return DirectX::XMFLOAT3();
}


void SwarmAgent::applyForce(const DirectX::XMFLOAT3& _force)
{
    acceleration = DirectX::Float3Add(acceleration, _force);
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
