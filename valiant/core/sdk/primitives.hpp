#pragma once
#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <cmath>
#include <algorithm>

namespace math_primitives
{
	inline const int s_width = GetSystemMetrics( 0 );
	inline const int s_height = GetSystemMetrics( 1 );

	inline const int s_width_center = s_width / 2;
	inline const int s_height_center = s_height / 2;

	class fvector {
	public:
		fvector( ) : x( 0.f ), y( 0.f ), z( 0.f ) {}

		fvector( double _x, double _y, double _z ) : x( _x ), y( _y ), z( _z ) {}

		~fvector( ) {}

		double x;
		double y;
		double z;

		inline double dot( fvector v ) {
			return x * v.x + y * v.y + z * v.z;
		}

		inline double distance( fvector v ) {
			return double( sqrtf( powf( v.x - x, 2.0 ) + powf( v.y - y, 2.0 ) + powf( v.z - z, 2.0 ) ) );
		}

		inline double length( ) {
			return sqrt( x * x + y * y + z * z );
		}

		fvector operator+( fvector v ) {
			return fvector( x + v.x, y + v.y, z + v.z );
		}

		fvector operator-( fvector v ) {
			return fvector( x - v.x, y - v.y, z - v.z );
		}

		fvector operator*( double number ) const {
			return fvector( x * number, y * number, z * number );
		}

		fvector operator/( double number ) const {
			return fvector( x / number, y / number, z / number );
		}

		fvector& operator*=( double number ) {
			x *= number;
			y *= number;
			z *= number;
			return *this;
		}

		void add_scale( const fvector& v, float scale ) {
			x += v.x * scale;
			y += v.y * scale;
			z += v.z * scale;
		}

		bool is_zero( ) {
			return x == 0 && y == 0 && z == 0;
		}
	};

	class fvector2d {
	public:
		double x, y;

		fvector2d( double x = 0.0, double y = 0.0 ) : x( x ), y( y ) {}

		fvector2d operator+( const fvector2d& v ) const {
			return fvector2d( x + v.x, y + v.y );
		}

		fvector2d operator-( const fvector2d& v ) const {
			return fvector2d( x - v.x, y - v.y );
		}

		fvector2d operator*( double scalar ) const {
			return fvector2d( x * scalar, y * scalar );
		}

		bool is_zero( ) const {
			return x == 0.0 && y == 0.0;
		}
	};

	class frotator {
	public:
		frotator( ) : pitch( 0 ), yaw( 0 ), roll( 0 ) { }
		frotator( double Pitch, double Yaw, double Roll ) : pitch( Pitch ), yaw( Yaw ), roll( Roll ) { }

		frotator operator + ( const frotator& other ) const { return { this->pitch + other.pitch, this->yaw + other.yaw, this->roll + other.roll }; }
		frotator operator - ( const frotator& other ) const { return { this->pitch - other.pitch, this->yaw - other.yaw, this->roll - other.roll }; }
		frotator operator * ( double offset ) const { return { this->pitch * offset, this->yaw * offset, this->roll * offset }; }
		frotator operator / ( double offset ) const { return { this->pitch / offset, this->yaw / offset, this->roll / offset }; }

		frotator& operator = ( const double other ) { this->pitch = other; this->yaw = other; this->roll = other; return *this; }
		frotator& operator *= ( const double other ) { this->pitch *= other; this->yaw *= other; this->roll *= other; return *this; }
		frotator& operator /= ( const double other ) { this->pitch /= other; this->yaw /= other; this->roll /= other; return *this; }

		frotator& operator = ( const frotator& other ) { this->pitch = other.pitch; this->yaw = other.yaw; this->roll = other.roll; return *this; }
		frotator& operator += ( const frotator& other ) { this->pitch += other.pitch; this->yaw += other.yaw; this->roll += other.roll; return *this; }
		frotator& operator -= ( const frotator& other ) { this->pitch -= other.pitch; this->yaw -= other.yaw; this->roll -= other.roll; return *this; }
		frotator& operator /= ( const frotator& other ) { this->pitch /= other.pitch; this->yaw /= other.yaw; this->roll /= other.roll; return *this; }

		operator bool( ) const { return this->pitch != 0 && this->yaw != 0 && this->roll != 0; }

		friend bool operator == ( const frotator& a, const frotator& b ) { return a.pitch == b.pitch && a.yaw == b.yaw && a.roll == b.roll; }
		friend bool operator != ( const frotator& a, const frotator& b ) { return !(a == b); }

		frotator get( ) const {
			return frotator( pitch, yaw, roll );
		}

		void set( double _Pitch, double _Yaw, double _Roll ) {
			pitch = _Pitch;
			yaw = _Yaw;
			roll = _Roll;
		}

		frotator normalize( ) const {
			frotator result = get( );

			if ( std::isfinite( result.pitch ) && std::isfinite( result.yaw ) && std::isfinite( result.roll ) ) {
				result.pitch = std::clamp( result.pitch, -89.0, 89.0 );
				result.yaw = std::clamp( result.yaw, -180.0, 180.0 );
				result.roll = 0.0;
			}

			return result;
		}

		double length( ) const {
			return std::sqrt( pitch * pitch + yaw * yaw + roll * roll );
		}

		double dot( const frotator& V ) const { return pitch * V.pitch + yaw * V.yaw + roll * V.roll; }

		double distance( const frotator& V ) const {
			return std::sqrt( std::pow( V.pitch - this->pitch, 2.0 ) + std::pow( V.yaw - this->yaw, 2.0 ) + std::pow( V.roll - this->roll, 2.0 ) );
		}

		double pitch;
		double yaw;
		double roll;
	};

	struct fplane : fvector
	{
		double w = 0;
	};
}

#endif