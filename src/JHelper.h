#pragma once

#include <DirectXMath.h>

#include <algorithm>

/* Various functions that don't belong in one specific place and are useful
 * in many areas.
 */
namespace JHelper
{
    int calculateIndex(const unsigned int _x, const unsigned int _y, const unsigned int _size_x);
    DirectX::XMINT2 calculateCoords(const unsigned int _index, const unsigned int _size_x);

    bool validIndex(const int _index, const int _array_size);

    float fisqrt(float _n);
}
