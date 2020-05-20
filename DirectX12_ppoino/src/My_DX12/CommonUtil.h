#pragma once
#include <Windows.h>
/**
* @file   CommonUtil.h
* @brief  共通で使える便利な定義とかを書いている　つもりです.
* @author licrosea
* @date 20200513
*/

/**
* @struct Vector2
* @brief  2次元ベクトルの構造体です.
*         ベクトルとしての使用にはもちろんのこと、サイズや位置指定にもどうぞ.
*/
struct Vector2
{
	float x;
	float y;

	// コンストラクタ.
	Vector2() : x( 0 ), y( 0 )
	{}
	Vector2( float i_x, float i_y ) : x( i_x ), y( i_y )
	{}

	// 演算子オーバーロード.
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
* @brief  2次元ベクトルのintバージョンです.
*         さらにサイズや位置指定に特化したほう...だと思います.
*/
struct Vector2Int
{
	int x;
	int y;

	// コンストラクタ.
	Vector2Int() : x( 0 ), y( 0 )
	{}
	Vector2Int( int i_x, int i_y ) : x( i_x ), y( i_y )
	{}
	// 下記コンストラクタは実質変換するやつ.
	Vector2Int( Vector2 i_obj ) : x( (int)i_obj.x ), y( (int)i_obj.y )
	{}

	// 演算子オーバーロード.
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

	// 下記は通常版Vector2用のオーバーロード.
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
* @brief 多重開放（≒NULL状態での解放）を防ぐ解放関数です.
* @param[in] obj 解放するオブジェクトを入れてください.
*/
template <class T> inline void SafeDelete(T** i_obj) {
	if( (*i_obj) == nullptr ) return;
	if( (*i_obj) == NULL    ) return; // 一応こっちの形式もチェック.

	delete (*i_obj);
	(*i_obj) = nullptr;
}

/**
* @fn void InitializeZero(T* i_obj)
* @brief 0初期化を行います.
* @param[in] obj 初期化するオブジェクトを入れてください.
*/
template <class T> inline void InitializeZero(T* i_obj)
{
	memset( i_obj, 0, sizeof( T ) );
}