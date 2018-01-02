#pragma once

#include <atomic>

#include <DirectXMath.h>

class InputHandler;

/* Simple structure to pass to objects that need to react to input, or tailor
 * their logic to the position of the main camera.
 */
struct GameData
{
    GameData()
        : exit(false)
        , input(nullptr)
        , camera_pos(0, 0, 0)
    {}

    std::atomic<bool> exit;

    InputHandler* input;
    DirectX::XMFLOAT3 camera_pos;

};
