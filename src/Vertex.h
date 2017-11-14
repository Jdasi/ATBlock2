#pragma once

#include <vector>

#include <DirectXMath.h>

struct CBPerObject
{
    DirectX::XMMATRIX wvp = DirectX::XMMatrixIdentity();
};

struct CBPerFrame
{
    std::vector<DirectX::XMMATRIX> agent_mats;
};

struct Vertex
{
    Vertex()
        : pos(0, 0, 0)
    {}

    Vertex(const float _x, const float _y, const float _z)
        : pos(_x, _y, _z)
    {}

    DirectX::XMFLOAT3 pos;
};
