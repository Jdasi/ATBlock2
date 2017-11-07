#pragma once

#include <windows.h>
#include <string>
#include <memory>

#include "GameData.h"
#include "Window.h"
#include "Renderer.h"
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
    void update();
    void render();

    HINSTANCE hinstance;
    Window window;
    Renderer renderer;

    // Debug.
    std::unique_ptr<Triangle> triangle;


};