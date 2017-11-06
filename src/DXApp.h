#pragma once

#include <windows.h>
#include <string>

#include "GameData.h"
#include "Window.h"
#include "Renderer.h"


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

};