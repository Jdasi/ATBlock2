#pragma once

#include <Windows.h>
#include <vector>

#include <DirectXMath.h>

#include "GameAction.h"
#include "Keybinding.h"

/* Class for handling input-specific messages that are forwarded by the operating system.
 */
class InputHandler
{
public:
    InputHandler();
    ~InputHandler() = default;

    void processMessage(const MSG& _msg);
    void lateTick();

    bool getKey(const int _key);
    bool getKeyDown(const int _key);
    bool getKeyUp(const int _key);

    bool getAction(const GameAction& _action);
    bool getActionDown(const GameAction& _action);
    bool getActionUp(const GameAction& _action);

    int getMouseDeltaX() const;
    int getMouseDeltaY() const;
    int getMouseDeltaZ() const;

private:
    void initKeyBindings();
    void sortKeyBindings();

    int actionToKey(const GameAction& _action) const;

    bool keys[256];
    bool prev_keys[256];

    POINT last_mouse_point;
    int mouse_delta_x;
    int mouse_delta_y;
    int mouse_delta_z;

    std::vector<Keybinding> key_bindings;

};
