#include <algorithm>
#include <iostream>

#include "InputHandler.h"


InputHandler::InputHandler()
{
    initKeyBindings();
}


void InputHandler::processMessage(MSG _msg)
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
    }
}


void InputHandler::lateTick()
{
    memcpy(prev_keys, keys, sizeof(bool) * 256);
}


bool InputHandler::getKey(int _key)
{
    _key = toupper(_key);
    return keys[_key] && prev_keys[_key];
}


bool InputHandler::getKeyDown(int _key)
{
    _key = toupper(_key);
    return keys[_key] && !prev_keys[_key];
}


bool InputHandler::getKeyUp(int _key)
{
    _key = toupper(_key);
    return !keys[_key] && prev_keys[_key];
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


void InputHandler::initKeyBindings()
{
    key_bindings.push_back({ GameAction::FORWARD, 'W' });
    key_bindings.push_back({ GameAction::BACKWARD, 'S' });
    key_bindings.push_back({ GameAction::LEFT, 'A' });
    key_bindings.push_back({ GameAction::RIGHT, 'D' });
    key_bindings.push_back({ GameAction::UP, 'E' });
    key_bindings.push_back({ GameAction::DOWN, 'Q' });
    key_bindings.push_back({ GameAction::ACCEPT, VK_RETURN });
    key_bindings.push_back({ GameAction::PAUSE, 'P' });
    key_bindings.push_back({ GameAction::QUIT, VK_ESCAPE });

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
