/**
 * @file   CWindow.h
 * @brief  CWindowクラス　宣言部.
 * @author licrosea.
 * @date   2020/03/22
 */

#pragma once

#include <Windows.h>
#include <string>
#include <functional>
#include <map>
#include <vector>

 /**
 * @typedef    FMessageCallback
 * @brief      ウィンドウメッセージを受け取った際の.
 *             コールバックラムダ関数定義です.
 *             （(クラス名):: を前置きするのがめんどいので、外部に定義しています）.
 */

typedef std::function< void(WPARAM, LPARAM) > FMessageCallback;

 /**
  * @class CWindow
  * @brief WinAPIのウィンドウクラスです.
  */
class CWindow final
{
private:
    // コピー作成 は取り急ぎなしとします.
    CWindow( CWindow& );

public:

    CWindow();
	virtual ~CWindow();

	/// ウィンドウを初期化.
	int Initialize( HINSTANCE i_hInstance, WNDPROC i_wndProc );

    /// ウィンドウ更新.
    int Update();

    /// ウィンドウ終了処理.
    void Finalize();

    /// ウィンドウハンドル取得.
    HWND GetHWnd() { return m_hWnd; }

    /// ウィンドウメッセージ処理.
    LRESULT CALLBACK OnWindowProc( HWND i_hwnd, UINT i_uMsg, WPARAM i_wParam, LPARAM i_lParam );

    /// メッセージ受信時に処理する関数を登録.
    void RegistCallBackOnReceiveMessage( UINT i_receiveMessage, FMessageCallback i_registCallback );
    /// 登録されているメッセージ受信関数の登録破棄.
    void UnRegistCallBackOnReceiveMessage( UINT i_receiveMessage );

    // ---------下記はいずれ...?--------------
    // void Resize();
    // bool SwitchFullScreenAndWindow();
    // EWindowType GetCurrentWindowType();

private:

    static const DWORD SMK_WINDOW_STYLE; // ウィンドウスタイル.

	HWND m_hWnd; // ウィンドウハンドル.

    std::map< UINT, FMessageCallback > m_callback_map; // コールバック関数のマップ.

};