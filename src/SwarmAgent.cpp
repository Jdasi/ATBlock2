#include "SwarmAgent.h"


SwarmAgent::SwarmAgent()
    : pos(0, 0, 0)
    , color(1, 1, 1, 1)
{
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
