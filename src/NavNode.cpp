#include <iostream>

#include "NavNode.h"


NavNode::NavNode(const float _matrix_scale)
    : pos(0, 0, 0)
    , node_index(0)
    , matrix_scale(_matrix_scale)
{
    setWalkable(true);
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


int NavNode::getNodeIndex() const
{
    return node_index;
}


void NavNode::setNodeIndex(const int _node_index)
{
    node_index = _node_index;
}


bool NavNode::isWalkable() const
{
    return walkable;
}


void NavNode::setWalkable(const bool _walkable)
{
    float val = _walkable ? 1.0f : 0.0f;
    walkable = _walkable;

    color = DirectX::XMFLOAT4(val, val, val, 1);
}


bool NavNode::containsPoint(const DirectX::XMFLOAT3& _pos) const
{
    float half_scale = matrix_scale / 2;

    float left = (pos.x * matrix_scale) - half_scale;
    float right = left + matrix_scale;
    float bottom = (pos.y * matrix_scale) - half_scale;
    float top = bottom + matrix_scale;

    if (right >= _pos.x &&
        left <= _pos.x &&
        top >= _pos.y &&
        bottom <= _pos.y)
    {
        std::cout << "Node: " << node_index << " contains point" << std::endl;
        return true;
    }

    return false;
}