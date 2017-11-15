#pragma once

#include <DirectXMath.h>

class InputHandler;

struct GameData
{
    GameData()
        : exit(false)
        , input(nullptr)
        , camera_pos(0, 0, 0)
    {}

    bool exit;

    InputHandler* input;
    DirectX::XMFLOAT3 camera_pos;

};
