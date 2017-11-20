#pragma once

#include <vector>

#include <DirectXMath.h>

struct CBPerObject
{
    DirectX::XMMATRIX obj_world = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX obj_rot = DirectX::XMMatrixIdentity();
    DirectX::XMFLOAT4 obj_color { 0, 0, 0, 1 };

    DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX proj = DirectX::XMMatrixIdentity();
};

struct Vertex
{
    Vertex()
        : pos(0, 0, 0)
        , color(0, 0, 0, 1)
    {}

    Vertex(const float _x, const float _y, const float _z)
        : pos(_x, _y, _z)
        , color(0, 0, 0, 1)
    {}

    Vertex(const float _x, const float _y, const float _z,
        const float _r, const float _g, const float _b, const float _a)
        : pos(_x, _y, _z)
        , color(_r, _g, _b, _a)
    {}

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;
};
