#pragma once

#include <limits>

namespace JMath
{

const float EPSILONF = std::numeric_limits<float>::epsilon();

int clamp(const int _original, const int _min, const int _max);
float clampf(const float _original, const float _min, const float _max);

float degToRad(const float _f);
float radToDeg(const float _f);

} // namespace JMath
