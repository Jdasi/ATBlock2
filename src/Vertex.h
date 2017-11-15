#pragma once

#include <vector>

#include <DirectXMath.h>

struct CBPerObject
{
    DirectX::XMMATRIX obj_world = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX obj_rot = DirectX::XMMatrixIdentity();

    DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX proj = DirectX::XMMatrixIdentity();
};

struct CBPerFrame
{
    DirectX::XMMATRIX node_mats;
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
