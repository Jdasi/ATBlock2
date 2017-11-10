#pragma once

#include <windows.h>
#include <memory>
#include <vector>

#include "Core.h"
#include "Camera.h"
#include "Triangle.h"
#include "Cube.h"

class DXApp
{
public:
    DXApp(HINSTANCE _hinstance);
    ~DXApp() = default;

    int main();

private:
    bool init();
    void initObjects();
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
    DrawData draw_data;

    // Objects.
    std::unique_ptr<Camera> camera;
    std::vector<std::unique_ptr<GameObject>> game_objects;

};
