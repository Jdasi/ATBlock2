#define WIN32_LEAN_AND_MEAN

#include "DXApp.h"


int WINAPI WinMain(__in HINSTANCE _hinstance, __in_opt HINSTANCE h_previnstance,
    __in LPSTR _lp_cmd_line, __in int _n_show_cmd)
{
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
