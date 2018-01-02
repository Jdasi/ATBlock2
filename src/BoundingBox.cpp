#include <cmath>

#include "BoundingBox.h"


BoundingBox::BoundingBox()
    : scale(0)
{
}


/* Updates the bounds of the BoundingBox, assuming that the origin of the box
 * is in the center.
 */
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


/* Updates the bounds of the BoundingBox, assuming that the origin of the box
 * is in the center.
 */
void BoundingBox::updateBounds(const DirectX::XMFLOAT3& _pos)
{
    updateBounds(_pos, scale);
}


/* Updates the bounds of the BoundingBox, assuming that the origin of the box
 * is in the center.
 */
void BoundingBox::updateScale(const float _scale)
{
    updateBounds(pos, _scale);
}


/* Returns true if the BoundingBox contains _pos, otherwise returns false.
 */
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


/* Returns the closest Edge of the BoundingBox to _pos.
 */
BoundingBox::Edge BoundingBox::closestEdge(const DirectX::XMFLOAT3& _pos) const
{
    // Precalculate all differences in the X and Y axis.
    float diff_left = abs(_pos.x - bounds.left);
    float diff_right = abs(_pos.x - bounds.right);
    float diff_top = abs(_pos.y - bounds.top);
    float diff_bottom = abs(_pos.y - bounds.bottom);

    Edge closest_edge = Edge::LEFT;

    if (diff_left < diff_right && diff_left < diff_top && diff_left < diff_bottom)
    {
        closest_edge = Edge::LEFT;
    }
    else if (diff_right < diff_left && diff_right < diff_top && diff_right < diff_bottom)
    {
        closest_edge = Edge::RIGHT;
    }
    else if (diff_top < diff_left && diff_top < diff_right && diff_top < diff_bottom)
    {
        closest_edge = Edge::TOP;
    }
    else if (diff_bottom < diff_left && diff_bottom < diff_right && diff_bottom < diff_top)
    {
        closest_edge = Edge::BOTTOM;
    }

    return closest_edge;
}


const Bounds& BoundingBox::getBounds() const
{
    return bounds;
}
