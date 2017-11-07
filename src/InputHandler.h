#pragma once

#include <Windows.h>
#include <vector>

#include "GameAction.h"
#include "Keybinding.h"

class InputHandler
{
public:
    InputHandler();
    ~InputHandler() = default;

    void processMessage(MSG _msg);
    void lateTick();

    bool getKey(int _key);
    bool getKeyDown(int _key);
    bool getKeyUp(int _key);

    bool getAction(const GameAction& _action);
    bool getActionDown(const GameAction& _action);
    bool getActionUp(const GameAction& _action);

private:
    void initKeyBindings();
    void sortKeyBindings();

    int actionToKey(const GameAction& _action) const;

    bool keys[256];
    bool prev_keys[256];

    std::vector<Keybinding> key_bindings;

};
