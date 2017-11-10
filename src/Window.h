#pragma once

#include <Windows.h>
#include <string>

class Window
{
public:
    Window(HINSTANCE _instance);
    ~Window() = default;

    bool init(const std::string& _title, const int _width, const int _height);

    HWND& getHandle();
    const UINT& getWidth() const;
    const UINT& getHeight() const;

    const float getWidthF() const;
    const float getHeightF() const;
    const float getAspectRatio() const;

private:
    HINSTANCE hinstance;

    std::string window_tile;
    UINT width;
    UINT height;
    HWND hwnd;
    DWORD wnd_style;

};