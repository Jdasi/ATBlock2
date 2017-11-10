#pragma once

class InputHandler;

struct GameData
{
    GameData()
        : exit(false)
    {}

    bool exit;

    InputHandler* input_handler;
};
