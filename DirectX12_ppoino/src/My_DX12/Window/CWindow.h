/**
 * @file   CWindow.h
 * @brief  CWindowクラス　宣言部.
 * @author licrosea.
 * @date   2020/03/22
 */

#pragma once

#include "WindowCommon.h"
#include "../CommonUtil.h"
#include <Windows.h>
#include <string>
#include <functional>
#include <map>
#include <vector>

#define MESSAGE_CALLBACK_ARGUMENTS ( WPARAM i_wParam, LPARAM i_lParam )
 /**
 * @typedef    FMessageCallback
 * @brief      ウィンドウメッセージを受け取った際の.
 *             コールバックラムダ関数定義です.
 */
using  FMessageCallback = std::function< void MESSAGE_CALLBACK_ARGUMENTS >;

 /**
  * @class CWindow
  * @brief WinAPIのウィンドウクラスです.
  */
class CWindow final
{
public: 
    
     /**
     * @enum  EWindowType.
     * @brief ウィンドウモードの列挙体です.
     */
    enum class EWindowType
    {
        eWIN_TYPE_NONE = 0,    /// なし（本来は使わない）.

        eWIN_TYPE_WINDOW,      /// ウィンドウモード.
        eWIN_TYPE_FULL_SCREEN, /// フルスクリーンモード.

        eWIN_TYPE_NUM,         /// モード総数(本来は使わない).
    };

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

    /// ウィンドウサイズ変更.
    void Resize( Vector2Int i_size );
  
    /// ウィンドウモードを変更します.
    bool ChangeWindowMode( EWindowType i_windowType );

    /// 現在のウィンドウモードを返します.
    inline EWindowType GetCurrentWindowType()
    {
        // 未生成時に限り、NONEを返す.
        if( m_hWnd == NULL ) return EWindowType::eWIN_TYPE_NONE;
        // それ以外は現状を返す.
        return m_currentType;
    }

private:

    // 当クラスでしか使わないのでここに定義.
    // ウィンドウ関係の操作をここに定義.
    using FWindowFunc = std::function< void() >;

    /// ウィンドウスタイルを取得.
    DWORD      GetWindowStyle( EWindowType i_windowType );
    /// クライアント領域をもとに、ウィンドウサイズの計算.
    Vector2Int CalcWindowSizeFromCilentSize( Vector2Int i_clientSize );

    static const DWORD SMK_WINDOW_STYLE;      // ウィンドウスタイル.
    static const DWORD SMK_FULL_SCREEN_STYLE; // ウィンドウスタイル.

	HWND m_hWnd; // ウィンドウハンドル.

    std::map< UINT, FMessageCallback > m_callbackMap; // コールバック関数のマップ.

    Vector2Int m_size; // ウィンドウサイズ.
    Vector2Int m_pos;  // ウィンドウポジション.

    Vector2Int m_displaySize;    // ディスプレイの大きさ（関数で取得する為、定数にしていません）.

    EWindowType m_currentType; // ウィンドウの表示モード.
};