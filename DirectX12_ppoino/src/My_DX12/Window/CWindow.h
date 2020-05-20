/**
 * @file   CWindow.h
 * @brief  CWindow�N���X�@�錾��.
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
public: 
    
     /**
     * @enum  EWindowType.
     * @brief �E�B���h�E���[�h�̗񋓑̂ł�.
     */
    enum class EWindowType
    {
        eWIN_TYPE_NONE = 0,    /// �Ȃ��i�{���͎g��Ȃ��j.

        eWIN_TYPE_WINDOW,      /// �E�B���h�E���[�h.
        eWIN_TYPE_FULL_SCREEN, /// �t���X�N���[�����[�h.

        eWIN_TYPE_NUM,         /// ���[�h����(�{���͎g��Ȃ�).
    };

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
    void Resize( Vector2Int i_size );
  
    /// �E�B���h�E���[�h��ύX���܂�.
    bool ChangeWindowMode( EWindowType i_windowType );

    /// ���݂̃E�B���h�E���[�h��Ԃ��܂�.
    inline EWindowType GetCurrentWindowType()
    {
        // ���������Ɍ���ANONE��Ԃ�.
        if( m_hWnd == NULL ) return EWindowType::eWIN_TYPE_NONE;
        // ����ȊO�͌����Ԃ�.
        return m_currentType;
    }

private:

    // ���N���X�ł����g��Ȃ��̂ł����ɒ�`.
    // �E�B���h�E�֌W�̑���������ɒ�`.
    using FWindowFunc = std::function< void() >;

    /// �E�B���h�E�X�^�C�����擾.
    DWORD      GetWindowStyle( EWindowType i_windowType );
    /// �N���C�A���g�̈�����ƂɁA�E�B���h�E�T�C�Y�̌v�Z.
    Vector2Int CalcWindowSizeFromCilentSize( Vector2Int i_clientSize );

    static const DWORD SMK_WINDOW_STYLE;      // �E�B���h�E�X�^�C��.
    static const DWORD SMK_FULL_SCREEN_STYLE; // �E�B���h�E�X�^�C��.

	HWND m_hWnd; // �E�B���h�E�n���h��.

    std::map< UINT, FMessageCallback > m_callbackMap; // �R�[���o�b�N�֐��̃}�b�v.

    Vector2Int m_size; // �E�B���h�E�T�C�Y.
    Vector2Int m_pos;  // �E�B���h�E�|�W�V����.

    Vector2Int m_displaySize;    // �f�B�X�v���C�̑傫���i�֐��Ŏ擾����ׁA�萔�ɂ��Ă��܂���j.

    EWindowType m_currentType; // �E�B���h�E�̕\�����[�h.
};