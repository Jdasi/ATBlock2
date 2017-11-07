#pragma once
#include "GameAction.h"


struct Keybinding
{
    Keybinding(const GameAction _action, const int _code)
        : action(_action)
        , code(_code)
    {
    }

    GameAction action = GameAction::NONE;
    int code = 0;
};
