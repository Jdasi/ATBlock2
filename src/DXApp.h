#pragma once

#include <windows.h>
#include <string>
#include <memory>

#include "GameData.h"
#include "Window.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "Triangle.h"


class DXApp
{
public:
    DXApp(HINSTANCE _hinstance);
    ~DXApp() = default;

    int main();

private:
    bool init();
    int run();

    void tick();
    void render();

    // Progam instance.
    HINSTANCE hinstance;

    // Systems.
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<InputHandler> input_handler;

    // Data.
    GameData game_data;

    // Debug.
    std::unique_ptr<Triangle> triangle;


};