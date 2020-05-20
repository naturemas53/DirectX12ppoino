/**
 * @file   CWindow.cpp
 * @brief  CWindowクラス　実装部.
 * @author licrosea.
 * @date   2020/04/04
 */
#include "CWindow.h"

/// ウィンドウスタイルの指定.
const DWORD CWindow::SMK_WINDOW_STYLE      = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
const DWORD CWindow::SMK_FULL_SCREEN_STYLE = WS_VISIBLE | WS_POPUP;

namespace
{
    /**
    * @brief  ディスプレイのサイズを取得します.
    * @return Vector2Int 中にはメインディスプレイの大きさが入っています.
    */
    inline Vector2Int GetDispraySize()
    {
        RECT rec;
        memset(&rec, 0, sizeof(RECT));
        HWND hDeskWnd = GetDesktopWindow(); //この関数でデスクトップのハンドルを取得.
        GetWindowRect(hDeskWnd, &rec);      //デスクトップのハンドルからその(画面の)大きさを取得.

        return Vector2Int(rec.right - rec.left, rec.bottom - rec.top);
    }
}

/// コンストラクタ.
CWindow::CWindow() :
    m_hWnd( NULL ),
    m_size( Vector2Int() ),
    m_pos ( Vector2Int() ),
    m_displaySize( Vector2Int() ),
    m_currentType( EWindowType::eWIN_TYPE_WINDOW )
{
    m_callbackMap.clear();
}

/// デストラクタ.
CWindow::~CWindow()
{
    this->Finalize();
}

/**
 * @brief ウィンドウ初期化.
 * @param [in](i_hInstance) WinMainからもらえるhInstanceがここで必要です.
 * @param [in](i_wndProc)   ウィンドウメッセージを受け取る関数ポインタを指定してください.
 * @return エラー時0以下を返します。それ以外は1以上を返します.
 */
int CWindow::Initialize( HINSTANCE i_hInstance, WNDPROC i_wndProc)
{
    // 最初にディスプレイの大きさを取得.
    m_displaySize = GetDispraySize();

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
                            this->GetWindowStyle( m_currentType ), // 周りに枠があるか、最小化ボタンがあるかなどの指定を行う.
                            m_pos.x, m_pos.y, m_size.x, m_size.y, // TODO : 初期位置や大きさは関数で変えられるように.
                            NULL, NULL, // それぞれ　親ウィンドウ　メニュー　どちらも使わないのでNULL.
                            i_hInstance, NULL // インスタンス　なんかのポインタ（ウィンドウ作成時のデータ？）.
                           );


    // 生成失敗.
    if ( m_hWnd == NULL )
    {
        return 0;
    }

    // ウィンドウ表示.
    ShowWindow( m_hWnd, SW_SHOW );

    // 生成OK.
    return 1;
}

/**
 * @brief ウィンドウ更新.
 * @return エラー時0以下を返します。それ以外は1以上を返します.
 * @details サイズ変更などのウィンドウ操作はこのタイミングで行われます.
 */
int CWindow::Update()
{
    UpdateWindow( m_hWnd );
    
    return 0;
}

/**
 * @brief ウィンドウ終了処理(これが呼ばれる前にPostQuitMessageが呼ばれている前提とします).
 * @return void.
 */
void CWindow::Finalize()
{
    m_hWnd = NULL;
}

/**
 * @brief ウィンドウメッセージ受け取り用の関数.
 * @param [in](i_hwnd) どのウィンドウハンドルからメッセージを受け取ったか.
 * @param [in](i_uMsg) メッセージ内容.
 * @param [in](i_wParam) パラメーター(UINTっぽい？).
 * @param [in](i_lPalam) パラメーター(LONGっぽい？).
 * @return 原則0を返してください　WinAPIになんか伝える時に変わるんじゃない？（適当）.
 * @details CWindowの関数を直接ウィンドウプロシージャに関連付けるのがめんどくさかったので.
 *          このように別に用意しています.
 *          CWindow::Initialize( , WNDPROC i_wndProc )のi_wndProcの中で.
 *          当関数を呼ぶつくりにしてください.
 *          (もちろん独自に実装してもOKです　ただしその場合、.
 *           当クラスのメッセージごとのコールバック機能は使えません).
 */
LRESULT CALLBACK CWindow::OnWindowProc(HWND i_hwnd, UINT i_uMsg, WPARAM i_wParam, LPARAM i_lParam)
{   
    // 登録されたコールバックの呼び出し.
    if( m_callbackMap.find( i_uMsg ) != m_callbackMap.end() )
    {
        m_callbackMap[ i_uMsg ]( i_wParam, i_lParam );
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

/**
 * @brief メッセージ受信時のコールバックを登録.
 * @param [in](i_receiveMessage) コールバックを呼んでほしいメッセージの種類を指定します.
 * @param [in](i_registCallback) コールバックを指定します.
 * @return void.
 * @details 1メッセージ１つしか設定できません。複数の処理を行いたい場合は.
 *          登録したコールバックの中で何とかしてください（投げやり）.
 */
void CWindow::RegistCallBackOnReceiveMessage( UINT i_receiveMessage, FMessageCallback i_registCallback )
{
    m_callbackMap[i_receiveMessage] = i_registCallback;
}

/**
 * @brief  メッセージ受信時のコールバックの登録解除.
 * @param [in](i_receiveMessage) コールバックを消すメッセージの種類を指定します.
 * @return void.
 */
void CWindow::UnRegistCallBackOnReceiveMessage( UINT i_receiveMessage )
{
    m_callbackMap.erase(i_receiveMessage);
}

/**
 * @brief ウィンドウサイズを再設定します.
 * @param [in](i_size) 指定するウィンドウの大きさ.
 * @return void.
 */
void CWindow::Resize( Vector2Int i_size )
{
    // ウィンドウが生成されていない.
    if (m_hWnd == NULL) return;
    // ウィンドウタイプが正しく設定されていない.
    if( m_currentType <= EWindowType::eWIN_TYPE_NONE || m_currentType >= EWindowType::eWIN_TYPE_NUM ) return;

    m_size = this->CalcWindowSizeFromCilentSize( ( m_currentType == EWindowType::eWIN_TYPE_WINDOW ) ? i_size : m_displaySize );

    // 親ウィンドウはいないのでなし 動かさない | Z値は考慮しない | WM_NCCALCSIZEを送る（≒サイズ変更による再描画？).
    SetWindowPos( m_hWnd, NULL, 0, 0, m_size.x, m_size.y, SWP_NOZORDER | SWP_FRAMECHANGED );
}

/**
 * @brief ウィンドウモードを変更します.
 * @param [in](i_windowType) 指定するウィンドウタイプ.
 * @return 正常に変更ができればtrue 失敗するとfalseを返します.
 */
bool CWindow::ChangeWindowMode( EWindowType  i_windowType )
{
   // ウィンドウが生成されていない.
   if (m_hWnd == NULL) return false;

   m_currentType = i_windowType;

   LONG res = SetWindowLong( m_hWnd, GWL_STYLE, this->GetWindowStyle( m_currentType ) );

   return res != 0;
}

/**
 * @brief  ウィンドウスタイルを取得します.
 * @return DWORD 様々なフラグを組み合わせたウィンドウスタイルが返ります.
 */
DWORD CWindow::GetWindowStyle( EWindowType i_windowType )
{
    DWORD style = 0;

    switch ( i_windowType )
    {
    case EWindowType::eWIN_TYPE_WINDOW:      style = SMK_WINDOW_STYLE;      break;
    case EWindowType::eWIN_TYPE_FULL_SCREEN: style = SMK_FULL_SCREEN_STYLE; break;
    default: break;
    }

    return style;
}

/**
 * @brief クライアント領域をもとに、ウィンドウサイズの計算.
 * @param [in](i_clientSize) 確保するクライアント領域.
 * @return void.
 */
Vector2Int CWindow::CalcWindowSizeFromCilentSize( Vector2Int i_clientSize )
{
    // ウィンドウが生成されていない.
    if (m_hWnd == NULL) return Vector2Int( 0, 0 );

    DWORD style = this->GetWindowStyle( m_currentType );
    RECT  rec;
    InitializeZero( &rec );
    rec.right  = i_clientSize.x;
    rec.bottom = i_clientSize.y;

    AdjustWindowRect( &rec, style, false );

    return Vector2Int( rec.right - rec.left, rec.bottom - rec.top );
}