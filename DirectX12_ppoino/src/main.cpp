#include <windows.h>
#include "My_DX12/Window/CWindow.h"
#include "My_DX12/Window/WindowCommon.h"

CWindow window;

void  KeyDown MESSAGE_CALLBACK_ARGUMENTS;

LRESULT CALLBACK WinProc WINDOW_PROC_ARGUMENTS
{
    return window.OnWindowProc( i_hwnd, i_uMsg, i_wParam, i_lParam );
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, 
    int nCmdShow)
{

    bool is_continue = true;
    FMessageCallback quit_callback = [&]( WPARAM i_wPalam, LPARAM i_lParam )
    {
        is_continue = false;
    };

    window.RegistCallBackOnReceiveMessage( WM_DESTROY, quit_callback );
    window.RegistCallBackOnReceiveMessage( WM_KEYDOWN, KeyDown );
    window.Initialize( hInstance, WinProc );
    window.Resize( 10, 10 );

    MSG msg;
    memset( &msg, 0, sizeof( MSG ) );


    do{
        while ( PeekMessage(&msg, window.GetHWnd(), 0, 0, PM_NOREMOVE) != 0 )
        {
            if( GetMessage( &msg, window.GetHWnd(), 0, 0 ) < 0 )
            {
                printf( "msg error.\n" );
                PostQuitMessage( 0 );
                is_continue = false;
                break;
            }
            DispatchMessage(&msg);
        }
        printf( "complete messages.\n" );
        window.Update();
    }
    while (is_continue);

    return 0;
}

void KeyDown MESSAGE_CALLBACK_ARGUMENTS
{
    window.Resize( 1280, 720 );
}