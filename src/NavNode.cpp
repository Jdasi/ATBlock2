#include <iostream>

#include <algorithm>

#include "NavNode.h"


NavNode::NavNode(const int _matrix_scale, const int _max_agents)
    : pos(0, 0, 0)
    , color(0, 0, 0, 1)
    , node_index(0)
    , matrix_scale(_matrix_scale)
    , distance(0)
    , world_pos(0, 0, 0)
    , flow_dir(0, 0, 0)
{
    setWalkable(true);

    agent_bin.reserve(_max_agents);
}


// Gets the node's unscaled position (use getWorldPos for its actual world position).
const DirectX::XMFLOAT3& NavNode::getPos() const
{
    return pos;
}


// Gets the node's scaled position. This is where the node is in world space.
const DirectX::XMFLOAT3& NavNode::getWorldPos() const
{
    return world_pos;
}


// Sets the node's unscaled position. The world position is automatically recalulcated.
void NavNode::setPos(const DirectX::XMFLOAT3& _pos)
{
    pos = _pos;
    world_pos = DirectX::Float3Mul(_pos, matrix_scale);
    bounding_box.updateBounds(pos, static_cast<float>(matrix_scale));
}


// Sets the node's unscaled position. The world position is automatically recalulcated.
void NavNode::setPos(const float _x, const float _y, const float _z)
{
    setPos(DirectX::XMFLOAT3(_x, _y, _z));
}


// Gets the index of the node in the NavNode array.
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


const std::vector<NavNode*>& NavNode::getAdjacentNeighbours() const
{
    return adjacent_neighbours;
}


void NavNode::setAdjacentNeighbours(const std::vector<NavNode*>& _adjacent_neighbours)
{
    adjacent_neighbours = _adjacent_neighbours;
}


const std::vector<NavNode*>& NavNode::getAllNeighbours() const
{
    return all_neighbours;
}


void NavNode::setAllNeighbours(const std::vector<NavNode*>& _all_neighbours)
{
    all_neighbours = _all_neighbours;
}


bool NavNode::containsPoint(const DirectX::XMFLOAT3& _pos) const
{
    return bounding_box.containsPoint(_pos);
}


BoundingBox::Edge NavNode::closestEdge(const DirectX::XMFLOAT3& _pos) const
{
    return bounding_box.closestEdge(_pos);
}


const Bounds& NavNode::getWorldBounds() const
{
    return bounding_box.getBounds();
}


int NavNode::getBinCount() const
{
    return agent_bin.size();
}


const std::vector<SwarmAgent*>& NavNode::getAgentBin() const
{
    return agent_bin;
}


// Add an agent to this node's bin.
void NavNode::addAgentPtr(SwarmAgent* _agent)
{
    agent_bin.push_back(_agent);
}


// Remove an agent from this node's bin.
void NavNode::removeAgentPtr(SwarmAgent* _agent)
{
    agent_bin.erase(std::remove_if(
        agent_bin.begin(),
        agent_bin.end(),
        [_agent](SwarmAgent* _a) { return _a == _agent; }),
        agent_bin.end());
}
