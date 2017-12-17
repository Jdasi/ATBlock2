#include "DXMathHelper.h"
#include "JHelper.h"


DirectX::XMFLOAT3 DirectX::Float3Add(const XMFLOAT3& _a, const XMFLOAT3& _b)
{
    float x = _a.x + _b.x;
    float y = _a.y + _b.y;
    float z = _a.z + _b.z;

    return DirectX::XMFLOAT3{ x, y , z };
}


DirectX::XMFLOAT3 DirectX::Float3SubtractBfromA(const XMFLOAT3& _a, const XMFLOAT3& _b)
{
    float x = _a.x - _b.x;
    float y = _a.y - _b.y;
    float z = _a.z - _b.z;

    return DirectX::XMFLOAT3{ x, y , z };
}


DirectX::XMFLOAT3 DirectX::Float3Mul(const XMFLOAT3& _a, const int _mul)
{
    float x = _a.x * _mul;
    float y = _a.y * _mul;
    float z = _a.z * _mul;

    return DirectX::XMFLOAT3{ x, y , z };
}


DirectX::XMFLOAT3 DirectX::Float3Mul(const XMFLOAT3& _a, const float _mul)
{
    float x = _a.x * _mul;
    float y = _a.y * _mul;
    float z = _a.z * _mul;

    return DirectX::XMFLOAT3{ x, y , z };
}


DirectX::XMFLOAT3 DirectX::Float3Div(const XMFLOAT3& _a, const int _div)
{
    float x = _a.x / _div;
    float y = _a.y / _div;
    float z = _a.z / _div;

    return DirectX::XMFLOAT3{ x, y , z };
}


DirectX::XMFLOAT3 DirectX::Float3Div(const XMFLOAT3& _a, const float _div)
{
    float x = _a.x / _div;
    float y = _a.y / _div;
    float z = _a.z / _div;

    return DirectX::XMFLOAT3{ x, y , z };
}


DirectX::XMFLOAT3 DirectX::Float3Normalized(const XMFLOAT3& _a)
{
    float mag = JHelper::fisqrt(Float3MagnitudeSquared(_a));

    float x = _a.x * mag;
    float y = _a.y * mag;
    float z = _a.z * mag;

    return DirectX::XMFLOAT3{ x, y , z };
}


DirectX::XMFLOAT3 DirectX::Float3DirectionAtoB(const XMFLOAT3& _a, const XMFLOAT3& _b)
{
    return DirectX::Float3Normalized(Float3SubtractBfromA(_b, _a));
}


float DirectX::Float3Magnitude(const XMFLOAT3& _a)
{
    return JHelper::fisqrt(_a.x * _a.x + _a.y * _a.y + _a.z * _a.z);
}


float DirectX::Float3MagnitudeSquared(const XMFLOAT3& _a)
{
    return (_a.x * _a.x + _a.y * _a.y + _a.z * _a.z);
}


float DirectX::Float3Distance(const XMFLOAT3& _a, const XMFLOAT3& _b)
{
    XMFLOAT3 diff = Float3SubtractBfromA(_a, _b);
    return JHelper::fisqrt((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));
}


float DirectX::Float3DistanceSquared(const XMFLOAT3& _a, const XMFLOAT3& _b)
{
    XMFLOAT3 diff = Float3SubtractBfromA(_a, _b);
    return (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
}
