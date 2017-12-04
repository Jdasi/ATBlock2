#pragma once

#include <DirectXMath.h>

namespace DirectX
{
    static const XMFLOAT3 Vector3Zero      = {  0.f,  0.f,  0.f };
    static const XMFLOAT3 Vector3One       = {  1.f,  1.f,  1.f };
    static const XMFLOAT3 Vector3Up        = {  0.f,  1.f,  0.f };
    static const XMFLOAT3 Vector3Down      = {  0.f, -1.f,  0.f };
    static const XMFLOAT3 Vector3Right     = {  1.f,  0.f,  0.f };
    static const XMFLOAT3 Vector3Left      = { -1.f,  0.f,  0.f };
    static const XMFLOAT3 Vector3Forward   = {  0.f,  0.f,  1.f };
    static const XMFLOAT3 Vector3Backward  = {  0.f,  0.f, -1.f };

    XMFLOAT3 Float3Add(const XMFLOAT3& _a, const XMFLOAT3& _b);
    XMFLOAT3 Float3SubtractBfromA(const XMFLOAT3& _a, const XMFLOAT3& _b);

    XMFLOAT3 Float3Mul(const XMFLOAT3& _a, const int _mul);
    XMFLOAT3 Float3Mul(const XMFLOAT3& _a, const float _mul);

    XMFLOAT3 Float3Div(const XMFLOAT3& _a, const int _div);
    XMFLOAT3 Float3Div(const XMFLOAT3& _a, const float _div);

    XMFLOAT3 Float3Normalized(const XMFLOAT3& _a);
    XMFLOAT3 Float3DirectionAtoB(const XMFLOAT3& _a, const XMFLOAT3& _b);

    float Float3Magnitude(const XMFLOAT3& _a);
    float Float3MagnitudeSquared(const XMFLOAT3& _a);

    float Float3Distance(const XMFLOAT3& _a, const XMFLOAT3& _b);
    float Float3DistanceSquared(const XMFLOAT3& _a, const XMFLOAT3& _b);
}
