#include <windows.h>
#include "My_DX12/Window/CWindow.h"
#include "My_DX12/Window/WindowCommon.h"

LRESULT CALLBACK WinProc WINDOW_PROC_ARGUMENTS
{
    return DefWindowProc(i_hwnd, i_uMsg, i_wParam, i_lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, 
    int nCmdShow)
{
    CWindow window;

    window.Initialize( hInstance, WinProc );

    MSG msg;
    memset( &msg, 0, sizeof( MSG ) );

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}