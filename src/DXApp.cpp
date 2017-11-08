#include <iostream>

#include <DirectXMath.h>

#include "DXApp.h"
#include "JTime.h"


// Forward declaration of Window Procedure.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


DXApp::DXApp(HINSTANCE _hinstance)
    : hinstance(_hinstance)
    , window(nullptr)
    , renderer(nullptr)
    , input_handler(nullptr)
{
}


int DXApp::main()
{
    if (!init())
        return -1;

    return run();
}


bool DXApp::init()
{
    window = std::make_unique<Window>(hinstance);
    if (!window->init("AT Block 2 Project", 800, 600))
        return false;

    renderer = std::make_unique<Renderer>(window.get());
    if (!renderer->init(DirectX::XMFLOAT4(0.2f, 0.2f, 0.6f, 1)))
        return false;

    input_handler = std::make_unique<InputHandler>();

    return true;
}


int DXApp::run()
{
    triangle = std::make_unique<Triangle>(renderer.get());
    square = std::make_unique<Square>(renderer.get());

    MSG msg = { 0 };
    while (!game_data.exit)
    {
        JTime::tick();

        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
        {
            game_data.exit = true;
        }
        else
        {
            input_handler->processMessage(msg);

            tick();
            render();

            // Needs to be done last to properly record prev key states.
            input_handler->lateTick();
        }
    }

    return static_cast<int>(msg.wParam);
}


void DXApp::tick()
{
    if (input_handler->getAction(GameAction::LEFT))
    {
        std::cout << "a held" << std::endl;
    }

    if (input_handler->getActionDown(GameAction::LEFT))
    {
        std::cout << "a down" << std::endl;
    }

    if (input_handler->getActionUp(GameAction::LEFT))
    {
        std::cout << "a up" << std::endl;
    }

    if (input_handler->getActionDown(GameAction::QUIT))
    {
        game_data.exit = true;
    }
}


void DXApp::render()
{
    renderer->beginFrame();

    // render all ...
    //triangle->draw(renderer.get());
    square->draw(renderer.get());

    renderer->endFrame();
}
