#pragma once

#include <vector>

#include "DXMathHelper.h"

class SwarmAgent;

class NavNode
{
public:
    NavNode(const int _matrix_scale);
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

    int getBinCount() const;
    std::vector<SwarmAgent*>& getAgentBin();
    void addAgentPtr(SwarmAgent* _agent);
    void removeAgentPtr(SwarmAgent* _agent);

private:
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;

    int node_index;
    int matrix_scale;
    bool walkable;
    int distance;

    DirectX::XMFLOAT3 world_pos;
    DirectX::XMFLOAT3 flow_dir;

    std::vector<NavNode*> adjacent_neighbours;
    std::vector<NavNode*> all_neighbours;

    std::vector<SwarmAgent*> agent_bin;

};
