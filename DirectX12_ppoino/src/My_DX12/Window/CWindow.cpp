/**
 * @file   CWindow.cpp
 * @brief  CWindow�N���X�@������.
 * @author licrosea.
 * @date   2020/04/04
 */
#include "CWindow.h"

/// �E�B���h�E�X�^�C���̎w��.
const DWORD CWindow::SMK_WINDOW_STYLE      = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
const DWORD CWindow::SMK_FULL_SCREEN_STYLE = WS_VISIBLE | WS_POPUP;

namespace
{
    /**
    * @brief  �f�B�X�v���C�̃T�C�Y���擾���܂�.
    * @return Vector2Int ���ɂ̓��C���f�B�X�v���C�̑傫���������Ă��܂�.
    */
    inline Vector2Int GetDispraySize()
    {
        RECT rec;
        memset(&rec, 0, sizeof(RECT));
        HWND hDeskWnd = GetDesktopWindow(); //���̊֐��Ńf�X�N�g�b�v�̃n���h�����擾.
        GetWindowRect(hDeskWnd, &rec);      //�f�X�N�g�b�v�̃n���h�����炻��(��ʂ�)�傫�����擾.

        return Vector2Int(rec.right - rec.left, rec.bottom - rec.top);
    }
}

/// �R���X�g���N�^.
CWindow::CWindow() :
    m_hWnd( NULL ),
    m_size( Vector2Int() ),
    m_pos ( Vector2Int() ),
    m_displaySize( Vector2Int() ),
    m_currentType( EWindowType::eWIN_TYPE_WINDOW )
{
    m_callbackMap.clear();
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
    // �ŏ��Ƀf�B�X�v���C�̑傫�����擾.
    m_displaySize = GetDispraySize();

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
                            this->GetWindowStyle( m_currentType ), // ����ɘg�����邩�A�ŏ����{�^�������邩�Ȃǂ̎w����s��.
                            m_pos.x, m_pos.y, m_size.x, m_size.y, // TODO : �����ʒu��傫���͊֐��ŕς�����悤��.
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
 * @param [in](i_size) �w�肷��E�B���h�E�̑傫��.
 * @return void.
 */
void CWindow::Resize( Vector2Int i_size )
{
    // �E�B���h�E����������Ă��Ȃ�.
    if (m_hWnd == NULL) return;
    // �E�B���h�E�^�C�v���������ݒ肳��Ă��Ȃ�.
    if( m_currentType <= EWindowType::eWIN_TYPE_NONE || m_currentType >= EWindowType::eWIN_TYPE_NUM ) return;

    m_size = this->CalcWindowSizeFromCilentSize( ( m_currentType == EWindowType::eWIN_TYPE_WINDOW ) ? i_size : m_displaySize );

    // �e�E�B���h�E�͂��Ȃ��̂łȂ� �������Ȃ� | Z�l�͍l�����Ȃ� | WM_NCCALCSIZE�𑗂�i���T�C�Y�ύX�ɂ��ĕ`��H).
    SetWindowPos( m_hWnd, NULL, 0, 0, m_size.x, m_size.y, SWP_NOZORDER | SWP_FRAMECHANGED );
}

/**
 * @brief �E�B���h�E���[�h��ύX���܂�.
 * @param [in](i_windowType) �w�肷��E�B���h�E�^�C�v.
 * @return ����ɕύX���ł����true ���s�����false��Ԃ��܂�.
 */
bool CWindow::ChangeWindowMode( EWindowType  i_windowType )
{
   // �E�B���h�E����������Ă��Ȃ�.
   if (m_hWnd == NULL) return false;

   m_currentType = i_windowType;

   LONG res = SetWindowLong( m_hWnd, GWL_STYLE, this->GetWindowStyle( m_currentType ) );

   return res != 0;
}

/**
 * @brief  �E�B���h�E�X�^�C�����擾���܂�.
 * @return DWORD �l�X�ȃt���O��g�ݍ��킹���E�B���h�E�X�^�C�����Ԃ�܂�.
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
 * @brief �N���C�A���g�̈�����ƂɁA�E�B���h�E�T�C�Y�̌v�Z.
 * @param [in](i_clientSize) �m�ۂ���N���C�A���g�̈�.
 * @return void.
 */
Vector2Int CWindow::CalcWindowSizeFromCilentSize( Vector2Int i_clientSize )
{
    // �E�B���h�E����������Ă��Ȃ�.
    if (m_hWnd == NULL) return Vector2Int( 0, 0 );

    DWORD style = this->GetWindowStyle( m_currentType );
    RECT  rec;
    InitializeZero( &rec );
    rec.right  = i_clientSize.x;
    rec.bottom = i_clientSize.y;

    AdjustWindowRect( &rec, style, false );

    return Vector2Int( rec.right - rec.left, rec.bottom - rec.top );
}