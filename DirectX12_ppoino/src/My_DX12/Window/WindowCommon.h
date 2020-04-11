#pragma once

/**
 * @file   WindowCommon.h
 * @brief  ウィンドウ系共通定義.
 * @author licrosea.
 * @date   2020/04/10
 */
#include <Windows.h>

// ウィンドウプロシージャの引数マクロ.
#define WINDOW_PROC_ARGUMENTS ( HWND i_hwnd, UINT i_uMsg, WPARAM i_wParam, LPARAM i_lParam )