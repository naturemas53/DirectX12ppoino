/**
 * @file   CWindow.h
 * @brief  CWindow�N���X�@�錾��.
 * @author licrosea.
 * @date   2020/03/22
 */

#pragma once

#include "WindowCommon.h"
#include <Windows.h>
#include <string>
#include <functional>
#include <map>
#include <vector>

#define MESSAGE_CALLBACK_ARGUMENTS ( WPARAM i_wParam, LPARAM i_lParam )
 /**
 * @typedef    FMessageCallback
 * @brief      �E�B���h�E���b�Z�[�W���󂯎�����ۂ�.
 *             �R�[���o�b�N�����_�֐���`�ł�.
 */
using  FMessageCallback = std::function< void MESSAGE_CALLBACK_ARGUMENTS >;

 /**
  * @class CWindow
  * @brief WinAPI�̃E�B���h�E�N���X�ł�.
  */
class CWindow final
{
private:
    // �R�s�[�쐬 �͎��}���Ȃ��Ƃ��܂�.
    CWindow( CWindow& );

public:

    CWindow();
	virtual ~CWindow();

	/// �E�B���h�E��������.
	int Initialize( HINSTANCE i_hInstance, WNDPROC i_wndProc );

    /// �E�B���h�E�X�V.
    int Update();

    /// �E�B���h�E�I������.
    void Finalize();

    /// �E�B���h�E�n���h���擾.
    HWND GetHWnd() { return m_hWnd; }

    /// �E�B���h�E���b�Z�[�W����.
    LRESULT CALLBACK OnWindowProc( HWND i_hwnd, UINT i_uMsg, WPARAM i_wParam, LPARAM i_lParam );

    /// ���b�Z�[�W��M���ɏ�������֐���o�^.
    void RegistCallBackOnReceiveMessage( UINT i_receiveMessage, FMessageCallback i_registCallback );
    /// �o�^����Ă��郁�b�Z�[�W��M�֐��̓o�^�j��.
    void UnRegistCallBackOnReceiveMessage( UINT i_receiveMessage );

    /// �E�B���h�E�T�C�Y�ύX.
    // TODO : ������xy���\���̂ɂ�����.
    void Resize( int i_width, int i_height );
  
    // ---------���L�͂�����...?--------------
    // bool SwitchFullScreenAndWindow();
    // EWindowType GetCurrentWindowType();

private:

    // ���N���X�ł����g��Ȃ��̂ł����ɒ�`.
    // �E�B���h�E�֌W�̑���������ɒ�`.
    using FWindowFunc = std::function< void() >;

    
    /// �E�B���h�E��������N�G�X�g�i��ɃN���X���݂̂Ŏg���܂��j.
    void RequestWindowFunc( FWindowFunc i_requestFunc );
    
    static const DWORD SMK_WINDOW_STYLE; // �E�B���h�E�X�^�C��.

	HWND m_hWnd; // �E�B���h�E�n���h��.

    std::map< UINT, FMessageCallback > m_callbackMap; // �R�[���o�b�N�֐��̃}�b�v.
    std::vector< FWindowFunc > m_bookedWindowFuncList; // �E�B���h�E����̃��N�G�X�g���X�g.

    int m_width;
    int m_height;
    int m_posX;
    int m_posY;
};