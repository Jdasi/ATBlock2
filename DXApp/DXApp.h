#pragma once

#include <windows.h>
#include <string>

#define WIN32_LEAN_AND_MEAN


class DXApp
{
public:
    DXApp(HINSTANCE _hinstance);
    ~DXApp();

    int run();

private:
    bool init();
    bool init_window();
    bool init_renderer();

    int main_loop();
    void update();
    void render();

    HWND h_app_wnd;
    HINSTANCE h_app_instance;
    UINT draw_width;
    UINT draw_height;
    std::string app_title;
    DWORD wnd_style;

};