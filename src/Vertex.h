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
        , color(1, 1, 1, 1)
    {}

    Vertex(const float _x, const float _y, const float _z)
        : pos(_x, _y, _z)
        , color(1, 1, 1, 1)
    {}

    Vertex(const float _x, const float _y, const float _z,
        const float _cr, const float _cg, const float _cb, const float _ca)
        : pos(_x, _y, _z)
        , color(_cr, _cg, _cb, _ca)
    {}

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;
};
