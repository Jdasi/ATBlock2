#pragma once

#include <Windows.h>
#include <string>

/* Encapsulates a Windows operating system application window.
 */
class Window
{
public:
    Window(HINSTANCE _instance);
    ~Window() = default;

    bool init(const std::string& _title, const int _width, const int _height);

    HWND& getHandle();
    const int& getWidth() const;
    const int& getHeight() const;

    const float getWidthF() const;
    const float getHeightF() const;
    const float getAspectRatio() const;

private:
    bool registerWindow();
    bool createWindowHandle(const std::string& _title);

    HINSTANCE hinstance;

    std::string window_tile;
    int width;
    int height;
    HWND hwnd;
    DWORD wnd_style;

};