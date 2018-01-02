#pragma once

#include "DXMathHelper.h"

/* Represents the extremeties of a rectangle.
 */
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

/* Contains information about the extremeties of a world-space rectangle.
 * This class should be frequently updated by a rectangular object that requires
 * collision-checking functionality.
 */
class BoundingBox
{
public:
    enum Edge
    {
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };

    BoundingBox();
    ~BoundingBox() = default;

    void updateBounds(const DirectX::XMFLOAT3& _pos, const float _scale);
    void updateBounds(const DirectX::XMFLOAT3& _pos);
    void updateScale(const float _scale);

    bool containsPoint(const DirectX::XMFLOAT3& _pos) const;
    BoundingBox::Edge closestEdge(const DirectX::XMFLOAT3& _pos) const;

    const Bounds& getBounds() const;

private:
    DirectX::XMFLOAT3 pos;
    float scale;

    Bounds bounds;

};
