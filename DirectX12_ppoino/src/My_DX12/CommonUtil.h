#pragma once
#include <Windows.h>
/**
* @file   CommonUtil.h
* @brief  ���ʂŎg����֗��Ȓ�`�Ƃ��������Ă���@����ł�.
* @author licrosea
* @date 20200513
*/

/**
* @struct Vector2
* @brief  2�����x�N�g���̍\���̂ł�.
*         �x�N�g���Ƃ��Ă̎g�p�ɂ͂������̂��ƁA�T�C�Y��ʒu�w��ɂ��ǂ���.
*/
struct Vector2
{
	float x;
	float y;

	// �R���X�g���N�^.
	Vector2() : x( 0 ), y( 0 )
	{}
	Vector2( float i_x, float i_y ) : x( i_x ), y( i_y )
	{}

	// ���Z�q�I�[�o�[���[�h.
	Vector2& operator =(const Vector2 i_rhs )
	{
		this->x = i_rhs.x;
		this->y = i_rhs.y;
		return *this;
	}
	Vector2& operator +( const Vector2 i_rhs )
	{
		this->x += i_rhs.x;
		this->y += i_rhs.y;
		return *this;
	}
	Vector2& operator -( const Vector2 i_rhs )
	{
		this->x -= i_rhs.x;
		this->y -= i_rhs.y;
		return *this;
	}
	const bool operator ==( const Vector2 i_rhs )
	{
		return ( this->x == i_rhs.x ) && ( this->y == i_rhs.y );
	}
};

/**
* @struct Vector2Int
* @brief  2�����x�N�g����int�o�[�W�����ł�.
*         ����ɃT�C�Y��ʒu�w��ɓ��������ق�...���Ǝv���܂�.
*/
struct Vector2Int
{
	int x;
	int y;

	// �R���X�g���N�^.
	Vector2Int() : x( 0 ), y( 0 )
	{}
	Vector2Int( int i_x, int i_y ) : x( i_x ), y( i_y )
	{}
	// ���L�R���X�g���N�^�͎����ϊ�������.
	Vector2Int( Vector2 i_obj ) : x( (int)i_obj.x ), y( (int)i_obj.y )
	{}

	// ���Z�q�I�[�o�[���[�h.
	Vector2Int& operator =( const Vector2Int i_rhs )
	{
		this->x = i_rhs.x;
		this->y = i_rhs.y;
		return *this;
	}
	Vector2Int& operator +( const Vector2Int i_rhs )
	{
		this->x += i_rhs.x;
		this->y += i_rhs.y;
		return *this;
	}
	Vector2Int& operator -( const Vector2Int i_rhs )
	{
		this->x -= i_rhs.x;
		this->y -= i_rhs.y;
		return *this;
	}
	const bool operator ==( const Vector2Int i_rhs )
	{
		return ( this->x == i_rhs.x ) && ( this->y == i_rhs.y );
	}

	// ���L�͒ʏ��Vector2�p�̃I�[�o�[���[�h.
	Vector2Int& operator =( const Vector2 i_rhs ) 
	{
		Vector2Int intRhs = ( i_rhs );
		( *this ) = intRhs;
		return *this;
	}
	Vector2Int& operator +(const Vector2 i_rhs)
	{
		Vector2Int intRhs = ( i_rhs );
		( *this ) = ( *this ) + intRhs;
		return *this;
	}
	Vector2Int& operator -(const Vector2 i_rhs)
	{
		Vector2Int intRhs = ( i_rhs );
		( *this ) = ( *this ) - intRhs;
		return *this;
	}
	const bool operator ==( const Vector2 i_rhs )
	{
		Vector2Int intRhs = ( i_rhs );
		return (*this) == intRhs;
	}
};

/**
* @fn void SafeDelete(T** obj)
* @brief ���d�J���i��NULL��Ԃł̉���j��h������֐��ł�.
* @param[in] obj �������I�u�W�F�N�g�����Ă�������.
*/
template <class T> inline void SafeDelete(T** i_obj) {
	if( (*i_obj) == nullptr ) return;
	if( (*i_obj) == NULL    ) return; // �ꉞ�������̌`�����`�F�b�N.

	delete (*i_obj);
	(*i_obj) = nullptr;
}

/**
* @fn void InitializeZero(T* i_obj)
* @brief 0���������s���܂�.
* @param[in] obj ����������I�u�W�F�N�g�����Ă�������.
*/
template <class T> inline void InitializeZero(T* i_obj)
{
	memset( i_obj, 0, sizeof( T ) );
}