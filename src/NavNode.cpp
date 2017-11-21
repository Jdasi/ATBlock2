#include <iostream>

#include "NavNode.h"


NavNode::NavNode(const float _matrix_scale)
    : pos(0, 0, 0)
    , color(0, 0, 0, 1)
    , node_index(0)
    , matrix_scale(_matrix_scale)
    , distance(0)
    , flow_dir(0, 0, 0)
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


// Walkable = white tile; Unwalkable = black tile.
void NavNode::setWalkable(const bool _walkable)
{
    float val = _walkable ? 1.0f : 0.0f;
    walkable = _walkable;

    color = DirectX::XMFLOAT4(val, val, val, 1);
}


int NavNode::getDistance() const
{
    return distance;
}


void NavNode::setDistance(const int _distance)
{
    distance = _distance;
}


const DirectX::XMFLOAT3 NavNode::getFlowDir() const
{
    return flow_dir;
}


void NavNode::resetFlowDir()
{
    flow_dir.x = 0;
    flow_dir.y = 0;
    flow_dir.z = 0;
}


void NavNode::setFlowDir(const DirectX::XMFLOAT3& _dir)
{
    flow_dir = _dir;
}


void NavNode::setFlowDir(const float _x, const float _y, const float _z)
{
    setFlowDir(DirectX::XMFLOAT3(_x, _y, _z));
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
        return true;
    }

    return false;
}
