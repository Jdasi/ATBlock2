#include <iostream>

#include "NavNode.h"


NavNode::NavNode(const float _matrix_scale)
    : pos(0, 0, 0)
    , color(1, 1, 1, 1)
    , matrix_scale(_matrix_scale)
{
}


const DirectX::XMFLOAT3& NavNode::getPos() const
{
    return pos;
}


void NavNode::setPos(const DirectX::XMFLOAT3& _pos)
{
    pos = _pos;
}


void NavNode::setPos(const float _x, const float _y, const float _z)
{
    setPos(DirectX::XMFLOAT3(_x, _y, _z));
}


void NavNode::adjustPos(const float _x, const float _y, const float _z)
{
    pos.x += _x;
    pos.y += _y;
    pos.z += _z;
}


const DirectX::XMFLOAT4& NavNode::getCol() const
{
    return color;
}


void NavNode::setColor(const DirectX::XMFLOAT4& _color)
{
    color = _color;
}


void NavNode::setColor(const float _r, const float _g, const float _b, const float _a)
{
    setColor(DirectX::XMFLOAT4(_r, _g, _b, _a));
}


bool NavNode::containsPoint(const DirectX::XMFLOAT3& _pos) const
{
    float half_scale = matrix_scale / 2;

    if ((pos.x - half_scale) + matrix_scale >= _pos.x &&
        (pos.x - half_scale) <= _pos.x &&
        (pos.y - half_scale) + matrix_scale >= _pos.y &&
        (pos.y - half_scale) <= _pos.y)
    {
        std::cout << "contains point" << std::endl;
        return true;
    }

    std::cout << "doesnt contain point" << std::endl;
    return false;
}
