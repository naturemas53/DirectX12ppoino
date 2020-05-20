#include <windows.h>
#include "My_DX12/Window/CWindow.h"
#include "My_DX12/Window/WindowCommon.h"
#include "My_DX12/CommonUtil.h"

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
    window.Resize( Vector2Int( 1280, 720 ) );

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
   using EWindowType = CWindow::EWindowType;

    EWindowType nextMode = EWindowType::eWIN_TYPE_NONE;
    Vector2Int  nextSize = Vector2Int();

    switch ( window.GetCurrentWindowType() )
    {
    case EWindowType::eWIN_TYPE_WINDOW:
    {
        nextMode = EWindowType::eWIN_TYPE_FULL_SCREEN;
        nextSize = Vector2Int( 1920, 1080 );
    }
    break;
    case EWindowType::eWIN_TYPE_FULL_SCREEN:
    {
        nextMode = EWindowType::eWIN_TYPE_WINDOW;
        nextSize = Vector2Int( 1280, 720 );
    }
    break;
    default:
        return;
    }

    window.ChangeWindowMode( nextMode );
    window.Resize( nextSize );
}