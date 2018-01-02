#include <algorithm>
#include <iostream>

#include "InputHandler.h"


InputHandler::InputHandler()
    : last_mouse_point()
    , mouse_delta_x(0)
    , mouse_delta_y(0)
    , mouse_delta_z(0)
{
    memset(&keys, false, sizeof(bool) * 256);
    memset(&prev_keys, false, sizeof(bool) * 256);

    initKeyBindings();
}


void InputHandler::processMessage(const MSG& _msg)
{
    switch (_msg.message)
    {
        case WM_KEYDOWN:
        {
            keys[_msg.wParam] = true;
        } break;

        case WM_KEYUP:
        {
            keys[_msg.wParam] = false;
        } break;

        case WM_MOUSEMOVE:
        {
            POINT p = { LOWORD(_msg.lParam), HIWORD(_msg.lParam) };

            mouse_delta_x = last_mouse_point.x - p.x;
            mouse_delta_y = last_mouse_point.y - p.y;

            last_mouse_point = p;
        } break;

        case WM_MOUSEWHEEL:
        {
            mouse_delta_z = GET_WHEEL_DELTA_WPARAM(_msg.wParam);
        } break;
    }
}


void InputHandler::lateTick()
{
    memcpy(prev_keys, keys, sizeof(bool) * 256);
    mouse_delta_z = 0;
}


bool InputHandler::getKey(const int _key)
{
    int actual = toupper(_key);
    return keys[actual] && prev_keys[actual];
}


bool InputHandler::getKeyDown(const int _key)
{
    int actual = toupper(_key);
    return keys[actual] && !prev_keys[actual];
}


bool InputHandler::getKeyUp(const int _key)
{
    int actual = toupper(_key);
    return !keys[actual] && prev_keys[actual];
}


bool InputHandler::getAction(const GameAction& _action)
{
    return getKey(actionToKey(_action));
}


bool InputHandler::getActionDown(const GameAction& _action)
{
    return getKeyDown(actionToKey(_action));
}


bool InputHandler::getActionUp(const GameAction& _action)
{
    return getKeyUp(actionToKey(_action));
}


int InputHandler::getMouseDeltaX() const
{
    return mouse_delta_x;
}


int InputHandler::getMouseDeltaY() const
{
    return mouse_delta_y;
}


int InputHandler::getMouseDeltaZ() const
{
    return mouse_delta_z;
}


void InputHandler::initKeyBindings()
{
    key_bindings.push_back({ GameAction::FORWARD,   'W' });
    key_bindings.push_back({ GameAction::BACKWARD,  'S' });
    key_bindings.push_back({ GameAction::LEFT,      'A' });
    key_bindings.push_back({ GameAction::RIGHT,     'D' });
    key_bindings.push_back({ GameAction::UP,        'E' });
    key_bindings.push_back({ GameAction::DOWN,      'Q' });
    key_bindings.push_back({ GameAction::ACCEPT,    VK_RETURN });
    key_bindings.push_back({ GameAction::PAUSE,     'P' });
    key_bindings.push_back({ GameAction::QUIT,      VK_ESCAPE });

    sortKeyBindings();
}


void InputHandler::sortKeyBindings()
{
    std::sort(key_bindings.begin(), key_bindings.end(),
        [](const auto& _lhs, const auto& _rhs)
    {
        return _lhs.action < _rhs.action;
    });
}


int InputHandler::actionToKey(const GameAction& _action) const
{
    auto entry = std::find_if(key_bindings.begin(), key_bindings.end(),
        [_action](const auto& _elem)
    {
        return _elem.action == _action;
    });

    if (entry != key_bindings.end())
        return entry->code;

    // This should only be thrown if the keybindings haven't been set up correctly.
    throw std::runtime_error("Unhandled GameAction in InputHandler");
}
