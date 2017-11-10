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
    static const XMFLOAT3 Vector3Forward   = {  0.f,  0.f, -1.f };
    static const XMFLOAT3 Vector3Backward  = {  0.f,  0.f,  1.f };
}
