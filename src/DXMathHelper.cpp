#include "DXMathHelper.h"
#include "JHelper.h"


DirectX::XMFLOAT3 DirectX::Float3Add(const XMFLOAT3& _a, const XMFLOAT3& _b)
{
    DirectX::XMFLOAT3 f;

    f.x = _a.x + _b.x;
    f.y = _a.y + _b.y;
    f.z = _a.z + _b.z;

    return f;
}


DirectX::XMFLOAT3 DirectX::Float3SubtractBfromA(const XMFLOAT3& _a, const XMFLOAT3& _b)
{
    DirectX::XMFLOAT3 f;

    f.x = _a.x - _b.x;
    f.y = _a.y - _b.y;
    f.z = _a.z - _b.z;

    return f;
}


DirectX::XMFLOAT3 DirectX::Float3Mul(const XMFLOAT3& _a, const int _mul)
{
    DirectX::XMFLOAT3 f;

    f.x = _a.x * _mul;
    f.y = _a.y * _mul;
    f.z = _a.z * _mul;

    return f;
}


DirectX::XMFLOAT3 DirectX::Float3Mul(const XMFLOAT3& _a, const float _mul)
{
    DirectX::XMFLOAT3 f;

    f.x = _a.x * _mul;
    f.y = _a.y * _mul;
    f.z = _a.z * _mul;

    return f;
}


DirectX::XMFLOAT3 DirectX::Float3Div(const XMFLOAT3& _a, const int _div)
{
    DirectX::XMFLOAT3 f;

    f.x = _a.x / _div;
    f.y = _a.y / _div;
    f.z = _a.z / _div;

    return f;
}


DirectX::XMFLOAT3 DirectX::Float3Div(const XMFLOAT3& _a, const float _div)
{
    DirectX::XMFLOAT3 f;

    f.x = _a.x / _div;
    f.y = _a.y / _div;
    f.z = _a.z / _div;

    return f;
}


DirectX::XMFLOAT3 DirectX::Float3Normalized(const XMFLOAT3& _a)
{
    DirectX::XMFLOAT3 f;

    float mag_sqr = Float3MagnitudeSquared(_a);
    float inv_sqrt = JHelper::fisqrt(mag_sqr);

    f.x = _a.x * inv_sqrt;
    f.y = _a.y * inv_sqrt;
    f.z = _a.z * inv_sqrt;

    return f;

    /*
    DirectX::XMFLOAT3 f;

    float mag = 1 / Float3Magnitude(_a);

    f.x = _a.x * mag;
    f.y = _a.y * mag;
    f.z = _a.z * mag;

    return f;
    */
}


DirectX::XMFLOAT3 DirectX::Float3DirectionAtoB(const XMFLOAT3& _a, const XMFLOAT3& _b)
{
    DirectX::XMFLOAT3 sub = Float3SubtractBfromA(_b, _a);
    sub = DirectX::Float3Normalized(sub);

    return sub;
}


float DirectX::Float3Magnitude(const XMFLOAT3& _a)
{
    return 1 / JHelper::fisqrt(_a.x * _a.x + _a.y * _a.y + _a.z * _a.z);
    //return sqrt(_a.x * _a.x + _a.y * _a.y + _a.z * _a.z);
}


float DirectX::Float3MagnitudeSquared(const XMFLOAT3& _a)
{
    return (_a.x * _a.x + _a.y * _a.y + _a.z * _a.z);
}


float DirectX::Float3Distance(const XMFLOAT3& _a, const XMFLOAT3& _b)
{
    XMFLOAT3 diff = Float3SubtractBfromA(_a, _b);
    return 1 / JHelper::fisqrt((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));
    //return sqrt((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));
}


float DirectX::Float3DistanceSquared(const XMFLOAT3& _a, const XMFLOAT3& _b)
{
    XMFLOAT3 diff = Float3SubtractBfromA(_a, _b);
    return (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
}
