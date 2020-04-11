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
    m_hWnd( NULL ),
    m_width( 0 ), m_height( 0 ),
    m_posX( 0 ), m_posY()
{
    m_callbackMap.clear();
    m_bookedWindowFuncList.clear();
}

/// �f�X�g���N�^.
CWindow::~CWindow()
{
    this->Finalize();
}

/**
 * @brief �E�B���h�E������.
 * @param [in](i_hInstance) WinMain������炦��hInstance�������ŕK�v�ł�.
 * @param [in](i_wndProc)   �E�B���h�E���b�Z�[�W���󂯎��֐��|�C���^���w�肵�Ă�������.
 * @return �G���[��0�ȉ���Ԃ��܂��B����ȊO��1�ȏ��Ԃ��܂�.
 */
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
                            m_posX, m_posY, m_width, m_height, // TODO : �����ʒu��傫���͊֐��ŕς�����悤��.
                            NULL, NULL, // ���ꂼ��@�e�E�B���h�E�@���j���[�@�ǂ�����g��Ȃ��̂�NULL.
                            i_hInstance, NULL // �C���X�^���X�@�Ȃ񂩂̃|�C���^�i�E�B���h�E�쐬���̃f�[�^�H�j.
                           );


    // �������s.
    if ( m_hWnd == NULL )
    {
        return 0;
    }

    // �E�B���h�E�\��.
    ShowWindow( m_hWnd, SW_SHOW );

    // ����OK.
    return 1;
}

/**
 * @brief �E�B���h�E�X�V.
 * @return �G���[��0�ȉ���Ԃ��܂��B����ȊO��1�ȏ��Ԃ��܂�.
 * @details �T�C�Y�ύX�Ȃǂ̃E�B���h�E����͂��̃^�C�~���O�ōs���܂�.
 */
int CWindow::Update()
{
    // �E�B���h�E������̂݁A���N�G�X�g���󂯕t���܂�.
    if ( m_hWnd != NULL && m_bookedWindowFuncList.size() > 0 )
    {
        // ���N�G�X�g���s.
        for ( FWindowFunc WindowFunc : m_bookedWindowFuncList )
        {
            WindowFunc();
        }
    }

    // ���N�G�X�g�N���A.
    m_bookedWindowFuncList.clear();

    UpdateWindow( m_hWnd );
    
    return 0;
}

/**
 * @brief �E�B���h�E�I������(���ꂪ�Ă΂��O��PostQuitMessage���Ă΂�Ă���O��Ƃ��܂�).
 * @return void.
 */
void CWindow::Finalize()
{
    m_hWnd = NULL;
}

/**
 * @brief �E�B���h�E���b�Z�[�W�󂯎��p�̊֐�.
 * @param [in](i_hwnd) �ǂ̃E�B���h�E�n���h�����烁�b�Z�[�W���󂯎������.
 * @param [in](i_uMsg) ���b�Z�[�W���e.
 * @param [in](i_wParam) �p�����[�^�[(UINT���ۂ��H).
 * @param [in](i_lPalam) �p�����[�^�[(LONG���ۂ��H).
 * @return ����0��Ԃ��Ă��������@WinAPI�ɂȂ񂩓`���鎞�ɕς��񂶂�Ȃ��H�i�K���j.
 * @details CWindow�̊֐��𒼐ڃE�B���h�E�v���V�[�W���Ɋ֘A�t����̂��߂�ǂ����������̂�.
 *          ���̂悤�ɕʂɗp�ӂ��Ă��܂�.
 *          CWindow::Initialize( , WNDPROC i_wndProc )��i_wndProc�̒���.
 *          ���֐����ĂԂ���ɂ��Ă�������.
 *          (�������Ǝ��Ɏ������Ă�OK�ł��@���������̏ꍇ�A.
 *           ���N���X�̃��b�Z�[�W���Ƃ̃R�[���o�b�N�@�\�͎g���܂���).
 */
LRESULT CALLBACK CWindow::OnWindowProc(HWND i_hwnd, UINT i_uMsg, WPARAM i_wParam, LPARAM i_lParam)
{   
    // �o�^���ꂽ�R�[���o�b�N�̌Ăяo��.
    if( m_callbackMap.find( i_uMsg ) != m_callbackMap.end() )
    {
        m_callbackMap[ i_uMsg ]( i_wParam, i_lParam );
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

/**
 * @brief ���b�Z�[�W��M���̃R�[���o�b�N��o�^.
 * @param [in](i_receiveMessage) �R�[���o�b�N���Ă�łق������b�Z�[�W�̎�ނ��w�肵�܂�.
 * @param [in](i_registCallback) �R�[���o�b�N���w�肵�܂�.
 * @return void.
 * @details 1���b�Z�[�W�P�����ݒ�ł��܂���B�����̏������s�������ꍇ��.
 *          �o�^�����R�[���o�b�N�̒��ŉ��Ƃ����Ă��������i�������j.
 */
void CWindow::RegistCallBackOnReceiveMessage( UINT i_receiveMessage, FMessageCallback i_registCallback )
{
    m_callbackMap[i_receiveMessage] = i_registCallback;
}

/**
 * @brief  ���b�Z�[�W��M���̃R�[���o�b�N�̓o�^����.
 * @param [in](i_receiveMessage) �R�[���o�b�N���������b�Z�[�W�̎�ނ��w�肵�܂�.
 * @return void.
 */
void CWindow::UnRegistCallBackOnReceiveMessage( UINT i_receiveMessage )
{
    m_callbackMap.erase(i_receiveMessage);
}

/**
 * @brief �E�B���h�E�T�C�Y���Đݒ肵�܂�.
 * @param [in](i_width)  �E�B���h�E�̕�.
 * @param [in](i_height) �E�B���h�E�̍���.
 * @return void.
 */
void CWindow::Resize( int i_width, int i_height )
{
    m_width  = i_width;
    m_height = i_height;

    FWindowFunc resizeFunc = [this]()
    {
        // �e�E�B���h�E�͂��Ȃ��̂łȂ� �������Ȃ� | Z�l�͍l�����Ȃ�.
        SetWindowPos( m_hWnd, NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER );
    };

    this->RequestWindowFunc( resizeFunc );
}

/**
 * @brief �E�B���h�E����̃��N�G�X�g���s���܂�.
 * @param [in](i_requestFunc)  ���N�G�X�g���鑀��.
 * @return void.
 */
void CWindow::RequestWindowFunc( FWindowFunc i_requestFunc )
{
    // �E�B���h�E����������Ă��Ȃ�.
    if (m_hWnd == NULL) return;

    m_bookedWindowFuncList.push_back( i_requestFunc );
}