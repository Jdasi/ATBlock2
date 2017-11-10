#include <iostream>

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

    initObjects();

    return true;
}


void DXApp::initObjects()
{
    camera = std::make_unique<Camera>(0.4f * 3.14f, window->getAspectRatio(), 0.1f, 1000.0f, DirectX::Vector3Up, DirectX::Vector3Zero);
    camera->setPos(0, 0, -10.0f);

    auto cube = std::make_unique<Cube>(renderer.get());
    game_objects.push_back(std::move(cube));

    auto triangle = std::make_unique<Triangle>(renderer.get());
    game_objects.push_back(std::move(triangle));

    // Game Data stuff.
    game_data.input_handler = input_handler.get();

    // Draw Data stuff.
    draw_data.renderer = renderer.get();
    draw_data.camera = camera.get();
}


int DXApp::run()
{
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
            // Handle input first.
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
    camera->tick(&game_data);

    for (auto& go : game_objects)
    {
        go->tick(&game_data);
    }

    if (input_handler->getActionDown(GameAction::QUIT))
    {
        game_data.exit = true;
    }
}


void DXApp::render()
{
    renderer->beginFrame();

    for (auto& go : game_objects)
    {
        go->draw(&draw_data);
    }

    renderer->endFrame();
}
