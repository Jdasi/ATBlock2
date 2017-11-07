#include "JMathHelper.h"

int JMath::clamp(const int _original, const int _min, const int _max)
{
    if (_original < _min) return _min;
    if (_original > _max) return _max;

    return _original;
}


float JMath::clampf(const float _original, const float _min, const float _max)
{
    if (_original < _min) return _min;
    if (_original > _max) return _max;

    return _original;
}


float JMath::degToRad(const float _f)
{
    return _f * 0.017453293f;
}


float JMath::radToDeg(const float _f)
{
    return _f * 57.29577951f;
}
