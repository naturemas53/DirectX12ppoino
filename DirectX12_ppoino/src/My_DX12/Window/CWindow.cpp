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
    m_hWnd( NULL ),
    m_width( 0 ), m_height( 0 ),
    m_posX( 0 ), m_posY()
{
    m_callbackMap.clear();
    m_bookedWindowFuncList.clear();
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
                            m_posX, m_posY, m_width, m_height, // TODO : 初期位置や大きさは関数で変えられるように.
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
    // ウィンドウ生成後のみ、リクエストを受け付けます.
    if ( m_hWnd != NULL && m_bookedWindowFuncList.size() > 0 )
    {
        // リクエスト実行.
        for ( FWindowFunc WindowFunc : m_bookedWindowFuncList )
        {
            WindowFunc();
        }
    }

    // リクエストクリア.
    m_bookedWindowFuncList.clear();

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
 * @param [in](i_width)  ウィンドウの幅.
 * @param [in](i_height) ウィンドウの高さ.
 * @return void.
 */
void CWindow::Resize( int i_width, int i_height )
{
    m_width  = i_width;
    m_height = i_height;

    FWindowFunc resizeFunc = [this]()
    {
        // 親ウィンドウはいないのでなし 動かさない | Z値は考慮しない.
        SetWindowPos( m_hWnd, NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER );
    };

    this->RequestWindowFunc( resizeFunc );
}

/**
 * @brief ウィンドウ操作のリクエストを行います.
 * @param [in](i_requestFunc)  リクエストする操作.
 * @return void.
 */
void CWindow::RequestWindowFunc( FWindowFunc i_requestFunc )
{
    // ウィンドウが生成されていない.
    if (m_hWnd == NULL) return;

    m_bookedWindowFuncList.push_back( i_requestFunc );
}