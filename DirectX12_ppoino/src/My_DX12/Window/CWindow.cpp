/**
 * @file   CWindow.cpp
 * @brief  CWindowクラス　実装部.
 * @author licrosea.
 * @date   2020/04/04
 */
#include "CWindow.h"

/// ウィンドウスタイルの指定.
const DWORD CWindow::SMK_WINDOW_STYLE = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

/// コンストラクタ.
CWindow::CWindow() :
    m_hWnd( NULL )
{
    m_callback_map.clear();
}

/// デストラクタ.
CWindow::~CWindow()
{}

int CWindow::Initialize( HINSTANCE i_hInstance, WNDPROC i_wndProc)
{

    std::string appName = "TestApp";
    WNDCLASS wc;

    // ウィンドウクラスの属性を設定
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = i_wndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = i_hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = appName.c_str();

    // ウィンドウクラスを登録
    if (!RegisterClass(&wc)) return 0;

    // ウィンドウ生成.
    m_hWnd = CreateWindowEx(
                            0, // Exで使える拡張スタイル　今回は使うことがないので0.
                            appName.c_str(), "TestWindow", // TODO : 後程定数とかで指定できるように.
                            SMK_WINDOW_STYLE, // HACK : 別な方法でもっと楽に指定できるようにするといいかも.
                            0, 0, 1280, 720, // TODO : 初期位置や大きさは関数で変えられるように.
                            NULL, NULL, // それぞれ　親ウィンドウ　メニュー　どちらも使わないのでNULL.
                            i_hInstance, NULL // インスタンス　なんかのポインタ（ウィンドウ作成時のデータ？）.
                           );


    if ( m_hWnd == NULL )
    {
        return 0;
    }

    // ウィンドウ表示.
    ShowWindow( m_hWnd, SW_SHOW );

    UpdateWindow( m_hWnd );

    return 1;
}

int CWindow::Update()
{
    // 工事中.
    return 0;
}

void CWindow::Finalize()
{

}

/**
 * @brief ウィンドウメッセージ受け取り用の関数.
 * @param [in](i_hwnd) どのウィンドウハンドルからメッセージを受け取ったか.
 * @param [in](i_uMsg) メッセージ内容.
 * @param [in](i_wParam) パラメーター(UINTっぽい？).
 * @param [in](i_lPalam) パラメーター(LONGっぽい？).
 * @return 原則0を返してください　WinAPIになんか伝える時に変わるんじゃない？（適当）.
 */

LRESULT CALLBACK CWindow::OnWindowProc(HWND i_hwnd, UINT i_uMsg, WPARAM i_wParam, LPARAM i_lParam)
{   
    // 登録されたコールバックの呼び出し.
    if( m_callback_map.find( i_uMsg ) != m_callback_map.end() )
    {
        m_callback_map[ i_uMsg ]( i_wParam, i_lParam );
    }

    // その他、メッセージごとの専用処理.
    switch (i_uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        this->Finalize();
        return 0;
    }

    return DefWindowProc(i_hwnd, i_uMsg, i_wParam, i_lParam);
}

void CWindow::RegistCallBackOnReceiveMessage( UINT i_receiveMessage, FMessageCallback i_registCallback )
{
    m_callback_map[i_receiveMessage] = i_registCallback;
}

void CWindow::UnRegistCallBackOnReceiveMessage( UINT i_receiveMessage )
{
    m_callback_map.erase(i_receiveMessage);
}