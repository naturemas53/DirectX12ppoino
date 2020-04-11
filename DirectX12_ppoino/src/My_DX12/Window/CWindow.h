/**
 * @file   CWindow.h
 * @brief  CWindow�N���X�@�錾��.
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
 * @brief      �E�B���h�E���b�Z�[�W���󂯎�����ۂ�.
 *             �R�[���o�b�N�����_�֐���`�ł�.
 *             �i(�N���X��):: ��O�u������̂��߂�ǂ��̂ŁA�O���ɒ�`���Ă��܂��j.
 */

typedef std::function< void(WPARAM, LPARAM) > FMessageCallback;

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

    // ---------���L�͂�����...?--------------
    // void Resize();
    // bool SwitchFullScreenAndWindow();
    // EWindowType GetCurrentWindowType();

private:

    static const DWORD SMK_WINDOW_STYLE; // �E�B���h�E�X�^�C��.

	HWND m_hWnd; // �E�B���h�E�n���h��.

    std::map< UINT, FMessageCallback > m_callback_map; // �R�[���o�b�N�֐��̃}�b�v.

};