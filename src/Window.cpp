#include "Window.h"


// Forward declaration of Window Procedure.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


Window::Window(HINSTANCE _instance)
    : hinstance(_instance)
    , width(0)
    , height(0)
    , hwnd(NULL)
    , wnd_style(WS_OVERLAPPEDWINDOW)
{
}


bool Window::init(const std::string & _title, const int _width, const int _height)
{
    width = _width;
    height = _height;

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.hInstance = hinstance;
    wcex.lpfnWndProc = WndProc;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "DXAPP";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        OutputDebugString("\n Error: Failed to create window class. \n");
        return false;
    }

    RECT r = { 0, 0, _width, _height };
    AdjustWindowRect(&r, wnd_style, FALSE);
    UINT width = r.right - r.left;
    UINT height = r.bottom - r.top;

    UINT x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
    UINT y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2);

    hwnd = CreateWindow("DXAPP", _title.c_str(), wnd_style,
        x, y, width, height, NULL, NULL, hinstance, NULL);

    if (!hwnd)
    {
        OutputDebugString("\n Error: Failed to create window. \n");
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);

    return true;
}


HWND& Window::getHandle()
{
    return hwnd;
}


const UINT& Window::getWidth() const
{
    return width;
}


const UINT& Window::getHeight() const
{
    return height;
}


const float Window::getWidthF() const
{
    return static_cast<float>(width);
}


const float Window::getHeightF() const
{
    return static_cast<float>(height);
}


const float Window::getAspectRatio() const
{
    return width / height;
}
