#pragma once

#include <vector>

#include <DirectXMath.h>

/* Data structure which encapsulates details about an object that is
 * due to be rendered.
 */
struct CBPerObject
{
    DirectX::XMMATRIX obj_world = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX obj_rot = DirectX::XMMatrixIdentity();
    DirectX::XMFLOAT4 obj_color { 0, 0, 0, 1 };

    DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX proj = DirectX::XMMatrixIdentity();
};

/* Data structure which represents a single point in a piece of geometry.
 * Input Layouts using this structure must follow the format:
 * 1. Position (Float3)
 * 2. Colour (Flor4)
 */
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
