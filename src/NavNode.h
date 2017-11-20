#pragma once

#include <DirectXMath.h>

class NavNode
{
public:
    NavNode(const float _matrix_scale);
    ~NavNode() = default;

    const DirectX::XMFLOAT3& getPos() const;
    void setPos(const DirectX::XMFLOAT3& _pos);
    void setPos(const float _x, const float _y, const float _z);

    int getNodeIndex() const;
    void setNodeIndex(const int _node_index);

    bool isWalkable() const;
    void setWalkable(const bool _walkable);

    bool containsPoint(const DirectX::XMFLOAT3& _pos) const;

private:
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;

    int node_index;
    float matrix_scale;
    bool walkable;

};
