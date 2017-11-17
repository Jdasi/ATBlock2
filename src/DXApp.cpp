#include <iostream>

#include "DXApp.h"
#include "JTime.h"


// Forward declaration of Window Procedure.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


DXApp::DXApp(HINSTANCE _hinstance)
    : hinstance(_hinstance)
    , window(nullptr)
    , renderer(nullptr)
{
}


DXApp::~DXApp()
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
    if (!window->init("AT Block 2 Project", 1024, 768))
        return false;

    renderer = std::make_unique<Renderer>(window.get());
    if (!renderer->init(DirectX::XMFLOAT4(0.2f, 0.2f, 0.6f, 1)))
        return false;

    initObjects();

    return true;
}


void DXApp::initObjects()
{
    input_handler = std::make_unique<InputHandler>();
    vbmm = std::make_unique<VBModelManager>(renderer.get());
    simulation_manager = std::make_unique<SimulationManager>(renderer.get(), vbmm->getModel("triangle"), 10000);

    camera = std::make_unique<Camera>(0.4f * 3.14f, window->getAspectRatio(), 0.1f, 1000.0f, DirectX::Vector3Up, DirectX::Vector3Zero);
    camera->setPos(0, 0, -50.0f);
    camera->setRelativeTarget(DirectX::Vector3Forward);

    // Game Data stuff.
    game_data.input = input_handler.get();

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

        // Clear message to avoid spam.
        msg = { 0 };
    }

    return static_cast<int>(msg.wParam);
}


void DXApp::tick()
{
    camera->tick(&game_data);
    simulation_manager->tick(&game_data);

    game_data.camera_pos = camera->getPos();

    if (input_handler->getActionDown(GameAction::QUIT))
    {
        game_data.exit = true;
    }
}


void DXApp::render()
{
    renderer->beginFrame();

    simulation_manager->draw(&draw_data);

    renderer->endFrame();
}
