/**
 * @file   CWindow.cpp
 * @brief  CWindow�N���X�@������.
 * @author licrosea.
 * @date   2020/04/04
 */
#include "CWindow.h"

/// �E�B���h�E�X�^�C���̎w��.
const DWORD CWindow::SMK_WINDOW_STYLE = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

/// �R���X�g���N�^.
CWindow::CWindow() :
    m_hWnd( NULL )
{
    m_callback_map.clear();
}

/// �f�X�g���N�^.
CWindow::~CWindow()
{}

int CWindow::Initialize( HINSTANCE i_hInstance, WNDPROC i_wndProc)
{

    std::string appName = "TestApp";
    WNDCLASS wc;

    // �E�B���h�E�N���X�̑�����ݒ�
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

    // �E�B���h�E�N���X��o�^
    if (!RegisterClass(&wc)) return 0;

    // �E�B���h�E����.
    m_hWnd = CreateWindowEx(
                            0, // Ex�Ŏg����g���X�^�C���@����͎g�����Ƃ��Ȃ��̂�0.
                            appName.c_str(), "TestWindow", // TODO : ����萔�Ƃ��Ŏw��ł���悤��.
                            SMK_WINDOW_STYLE, // HACK : �ʂȕ��@�ł����Ɗy�Ɏw��ł���悤�ɂ���Ƃ�������.
                            0, 0, 1280, 720, // TODO : �����ʒu��傫���͊֐��ŕς�����悤��.
                            NULL, NULL, // ���ꂼ��@�e�E�B���h�E�@���j���[�@�ǂ�����g��Ȃ��̂�NULL.
                            i_hInstance, NULL // �C���X�^���X�@�Ȃ񂩂̃|�C���^�i�E�B���h�E�쐬���̃f�[�^�H�j.
                           );


    if ( m_hWnd == NULL )
    {
        return 0;
    }

    // �E�B���h�E�\��.
    ShowWindow( m_hWnd, SW_SHOW );

    UpdateWindow( m_hWnd );

    return 1;
}

int CWindow::Update()
{
    // �H����.
    return 0;
}

void CWindow::Finalize()
{

}

/**
 * @brief �E�B���h�E���b�Z�[�W�󂯎��p�̊֐�.
 * @param [in](i_hwnd) �ǂ̃E�B���h�E�n���h�����烁�b�Z�[�W���󂯎������.
 * @param [in](i_uMsg) ���b�Z�[�W���e.
 * @param [in](i_wParam) �p�����[�^�[(UINT���ۂ��H).
 * @param [in](i_lPalam) �p�����[�^�[(LONG���ۂ��H).
 * @return ����0��Ԃ��Ă��������@WinAPI�ɂȂ񂩓`���鎞�ɕς��񂶂�Ȃ��H�i�K���j.
 */

LRESULT CALLBACK CWindow::OnWindowProc(HWND i_hwnd, UINT i_uMsg, WPARAM i_wParam, LPARAM i_lParam)
{   
    // �o�^���ꂽ�R�[���o�b�N�̌Ăяo��.
    if( m_callback_map.find( i_uMsg ) != m_callback_map.end() )
    {
        m_callback_map[ i_uMsg ]( i_wParam, i_lParam );
    }

    // ���̑��A���b�Z�[�W���Ƃ̐�p����.
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