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

    if (!registerWindow())
        return false;

    if (!createWindowHandle(_title))
        return false;

    ShowWindow(hwnd, SW_SHOW);

    return true;
}


bool Window::registerWindow()
{
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
        OutputDebugString("\n Error: Failed to register window. \n");
        return false;
    }

    return true;
}


bool Window::createWindowHandle(const std::string& _title)
{
    RECT r = { 0, 0, width, height };
    AdjustWindowRect(&r, wnd_style, FALSE);
    UINT rect_width = r.right - r.left;
    UINT rect_height = r.bottom - r.top;

    UINT x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (rect_width / 2);
    UINT y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (rect_height / 2);

    hwnd = CreateWindow("DXAPP", _title.c_str(), wnd_style,
        x, y, rect_width, rect_height, NULL, NULL, hinstance, NULL);

    if (!hwnd)
    {
        OutputDebugString("\n Error: Failed to create window handle. \n");
        return false;
    }

    return true;
}


HWND& Window::getHandle()
{
    return hwnd;
}


const int& Window::getWidth() const
{
    return width;
}


const int& Window::getHeight() const
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
    return getWidthF() / getHeightF();
}
