#include <cmath>

#include "BoundingBox.h"


BoundingBox::BoundingBox()
    : scale(0)
{
}


void BoundingBox::updateBounds(const DirectX::XMFLOAT3& _pos, const float _scale)
{
    pos = _pos;
    scale = _scale;

    float half_scale = static_cast<float>(scale) / 2;

    bounds.left = (pos.x * scale) - half_scale;
    bounds.right = bounds.left + scale;
    bounds.bottom = (pos.y * scale) - half_scale;
    bounds.top = bounds.bottom + scale;
}


void BoundingBox::updateBounds(const DirectX::XMFLOAT3& _pos)
{
    updateBounds(_pos, scale);
}


void BoundingBox::updateScale(const float _scale)
{
    updateBounds(pos, _scale);
}


bool BoundingBox::containsPoint(const DirectX::XMFLOAT3& _pos) const
{
    if (bounds.right >= _pos.x &&
        bounds.left <= _pos.x &&
        bounds.top >= _pos.y &&
        bounds.bottom <= _pos.y)
    {
        return true;
    }

    return false;
}


BoxEdge BoundingBox::closestEdge(const DirectX::XMFLOAT3& _pos) const
{
    float diff_left = abs(_pos.x - bounds.left);
    float diff_right = abs(_pos.x - bounds.right);
    float diff_top = abs(_pos.y - bounds.top);
    float diff_bottom = abs(_pos.y - bounds.bottom);

    BoxEdge closest_edge = BoxEdge::LEFT;

    if (diff_left < diff_right && diff_left < diff_top && diff_left < diff_bottom)
    {
        closest_edge = BoxEdge::LEFT;
    }
    else if (diff_right < diff_left && diff_right < diff_top && diff_right < diff_bottom)
    {
        closest_edge = BoxEdge::RIGHT;
    }
    else if (diff_top < diff_left && diff_top < diff_right && diff_top < diff_bottom)
    {
        closest_edge = BoxEdge::TOP;
    }
    else if (diff_bottom < diff_left && diff_bottom < diff_right && diff_bottom < diff_top)
    {
        closest_edge = BoxEdge::BOTTOM;
    }

    return closest_edge;
}


const Bounds& BoundingBox::getBounds() const
{
    return bounds;
}
