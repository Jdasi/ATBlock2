#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include "DXApp.h"


int WINAPI WinMain(__in HINSTANCE _hinstance, __in_opt HINSTANCE h_previnstance,
    __in LPSTR _lp_cmd_line, __in int _n_show_cmd)
{
#ifdef DEBUG
    // Add a Debug Console to which cout is directed whilst in DEBUG.
    if (AllocConsole()) {
        freopen("CONOUT$", "w", stdout);
        SetConsoleTitle("Debug Console");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    }
#endif

    DXApp app(_hinstance);
    return app.main();
}


LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
    switch (_msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(_hwnd, _msg, _wparam, _lparam);
    }
}
