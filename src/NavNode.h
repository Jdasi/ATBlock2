#pragma once

#include <vector>

#include "DXMathHelper.h"
#include "BoundingBox.h"

class SwarmAgent;

/* Represents a grid tile for visualisation and spatial partioning,
 * and a graph node for navigation systems.
 */
class NavNode
{
public:
    NavNode(const int _matrix_scale, const int _max_agents);
    ~NavNode() = default;

    const DirectX::XMFLOAT3& getPos() const;
    const DirectX::XMFLOAT3& getWorldPos() const;
    void setPos(const DirectX::XMFLOAT3& _pos);
    void setPos(const float _x, const float _y, const float _z);

    int getNodeIndex() const;
    void setNodeIndex(const int _node_index);

    bool isWalkable() const;
    void setWalkable(const bool _walkable);

    int getDistance() const;
    void setDistance(const int _distance);

    const DirectX::XMFLOAT3 getFlowDir() const;
    void resetFlowDir();
    void setFlowDir(const DirectX::XMFLOAT3& _dir);
    void setFlowDir(const float _x, const float _y, const float _z);

    const std::vector<NavNode*>& getAdjacentNeighbours() const;
    void setAdjacentNeighbours(const std::vector<NavNode*>& _adjacent_neighbours);

    const std::vector<NavNode*>& getAllNeighbours() const;
    void setAllNeighbours(const std::vector<NavNode*>& _all_neighbours);

    bool containsPoint(const DirectX::XMFLOAT3& _pos) const;
    BoundingBox::Edge closestEdge(const DirectX::XMFLOAT3& _pos) const;
    const Bounds& getWorldBounds() const;

    int getBinCount() const;
    const std::vector<SwarmAgent*>& getAgentBin() const;
    void addAgentPtr(SwarmAgent* _agent);
    void removeAgentPtr(SwarmAgent* _agent);

private:
    DirectX::XMFLOAT3 pos;      // Where the node is only in relation to other nodes.
    DirectX::XMFLOAT4 color;    // Colour of grid tile visualisation.

    int node_index;             // This node's index in the node array.
    int matrix_scale;           // Scale of the grid.
    bool walkable;              // Is this tile traversable?
    int distance;               // Used in navigation evaluation cycles.

    DirectX::XMFLOAT3 world_pos;    // Where the node is in world space.
    DirectX::XMFLOAT3 flow_dir;     // Which direction this node is pointing.

    BoundingBox bounding_box;       // Collision volume of the node.

    std::vector<NavNode*> adjacent_neighbours;  // Up, Down, Left, Right Node neighbours.
    std::vector<NavNode*> all_neighbours;       // Omni-directional Node neighbours.

    std::vector<SwarmAgent*> agent_bin; // All the agents currently inside this node.

};
