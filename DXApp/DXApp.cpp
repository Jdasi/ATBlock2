#include "DXApp.h"


// Forward declaration of Window Procedure.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


DXApp::DXApp(HINSTANCE _hinstance)
    : h_app_instance(_hinstance)
    , h_app_wnd(NULL)
    , draw_width(800)
    , draw_height(600)
    , app_title("AT Block 2 Project")
    , wnd_style(WS_OVERLAPPEDWINDOW)
{
}


DXApp::~DXApp()
{
}


int DXApp::run()
{
    if (!init())
        return 1;

    return main_loop();
}


bool DXApp::init()
{
    if (!init_window())
        return false;

    if (!init_renderer())
        return false;

    return true;
}


bool DXApp::init_window()
{
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.hInstance = h_app_instance;
    wcex.lpfnWndProc = WndProc;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "DXAPPWNDCLASS";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        OutputDebugString("\n Error: Failed to create window class. \n");
        return false;
    }

    RECT r = {0, 0, draw_width, draw_height};
    AdjustWindowRect(&r, wnd_style, FALSE);
    UINT width = r.right - r.left;
    UINT height = r.bottom - r.top;

    UINT x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
    UINT y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2);

    h_app_wnd = CreateWindow("DXAPPWNDCLASS", app_title.c_str(), wnd_style,
        x, y, width, height, NULL, NULL, h_app_instance, NULL);

    if (!h_app_wnd)
    {
        OutputDebugString("\n Error: Failed to create window. \n");
        return false;
    }

    ShowWindow(h_app_wnd, SW_SHOW);

    return true;
}


bool DXApp::init_renderer()
{
    return true;
}


int DXApp::main_loop()
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
}
