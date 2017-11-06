#include "DXApp.h"
#include "JMath.h"


// Forward declaration of Window Procedure.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


DXApp::DXApp(HINSTANCE _hinstance)
    : hinstance(_hinstance)
    , window(_hinstance)
    , renderer(window)
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
    if (!window.init("AT Block 2 Project", 800, 600))
        return false;

    if (!renderer.init(JMath::Vector4(255, 0, 0, 0)))
        return false;

    return true;
}


int DXApp::run()
{
    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            update();
            render();
        }
    }

    return static_cast<int>(msg.wParam);
}


void DXApp::update()
{
}


void DXApp::render()
{
    renderer.beginFrame();

    // render all ...

    renderer.endFrame();
}
