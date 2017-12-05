#pragma once

#include "DXMathHelper.h"

struct Bounds
{
    Bounds()
        : left(0)
        , right(0)
        , top(0)
        , bottom(0)
    {
    }

    float left;
    float right;
    float top;
    float bottom;
};

enum BoxEdge
{
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

class BoundingBox
{
public:
    BoundingBox();
    ~BoundingBox() = default;

    void updateBounds(const DirectX::XMFLOAT3& _pos, const float _scale);
    void updateBounds(const DirectX::XMFLOAT3& _pos);
    void updateScale(const float _scale);

    bool containsPoint(const DirectX::XMFLOAT3& _pos) const;
    BoxEdge closestEdge(const DirectX::XMFLOAT3& _pos) const;

    const Bounds& getBounds() const;

private:
    DirectX::XMFLOAT3 pos;
    float scale;

    Bounds bounds;

};
