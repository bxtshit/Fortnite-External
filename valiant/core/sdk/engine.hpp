#pragma once
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "../sdk/primitives.hpp"

namespace uengine
{
	struct ftransform
	{
		math_primitives::fplane rot;
		math_primitives::fvector translation;
		char pad[ 4 ];
		math_primitives::fvector scale;
		char pad1[ 4 ];

		inline D3DMATRIX to_matrix_with_scale( )
		{
			D3DMATRIX m;
			m._41 = translation.x;
			m._42 = translation.y;
			m._43 = translation.z;

			double x2 = rot.x + rot.x;
			double y2 = rot.y + rot.y;
			double z2 = rot.z + rot.z;

			double xx2 = rot.x * x2;
			double yy2 = rot.y * y2;
			double zz2 = rot.z * z2;
			m._11 = (1.0f - (yy2 + zz2)) * scale.x;
			m._22 = (1.0f - (xx2 + zz2)) * scale.y;
			m._33 = (1.0f - (xx2 + yy2)) * scale.z;

			double yz2 = rot.y * z2;
			double wx2 = rot.w * x2;
			m._32 = (yz2 - wx2) * scale.z;
			m._23 = (yz2 + wx2) * scale.y;

			double xy2 = rot.x * y2;
			double wz2 = rot.w * z2;
			m._21 = (xy2 - wz2) * scale.y;
			m._12 = (xy2 + wz2) * scale.x;

			double xz2 = rot.x * z2;
			double wy2 = rot.w * y2;
			m._31 = (xz2 + wy2) * scale.z;
			m._13 = (xz2 - wy2) * scale.x;

			m._14 = 0.0f;
			m._24 = 0.0f;
			m._34 = 0.0f;
			m._44 = 1.0f;

			return m;
		}
	};

	inline D3DMATRIX matrix_multiplication( D3DMATRIX matrix1, D3DMATRIX matrix2 )
	{
		D3DMATRIX result;
		result._11 = matrix1._11 * matrix2._11 + matrix1._12 * matrix2._21 + matrix1._13 * matrix2._31 + matrix1._14 * matrix2._41;
		result._12 = matrix1._11 * matrix2._12 + matrix1._12 * matrix2._22 + matrix1._13 * matrix2._32 + matrix1._14 * matrix2._42;
		result._13 = matrix1._11 * matrix2._13 + matrix1._12 * matrix2._23 + matrix1._13 * matrix2._33 + matrix1._14 * matrix2._43;
		result._14 = matrix1._11 * matrix2._14 + matrix1._12 * matrix2._24 + matrix1._13 * matrix2._34 + matrix1._14 * matrix2._44;
		result._21 = matrix1._21 * matrix2._11 + matrix1._22 * matrix2._21 + matrix1._23 * matrix2._31 + matrix1._24 * matrix2._41;
		result._22 = matrix1._21 * matrix2._12 + matrix1._22 * matrix2._22 + matrix1._23 * matrix2._32 + matrix1._24 * matrix2._42;
		result._23 = matrix1._21 * matrix2._13 + matrix1._22 * matrix2._23 + matrix1._23 * matrix2._33 + matrix1._24 * matrix2._43;
		result._24 = matrix1._21 * matrix2._14 + matrix1._22 * matrix2._24 + matrix1._23 * matrix2._34 + matrix1._24 * matrix2._44;
		result._31 = matrix1._31 * matrix2._11 + matrix1._32 * matrix2._21 + matrix1._33 * matrix2._31 + matrix1._34 * matrix2._41;
		result._32 = matrix1._31 * matrix2._12 + matrix1._32 * matrix2._22 + matrix1._33 * matrix2._32 + matrix1._34 * matrix2._42;
		result._33 = matrix1._31 * matrix2._13 + matrix1._32 * matrix2._23 + matrix1._33 * matrix2._33 + matrix1._34 * matrix2._43;
		result._34 = matrix1._31 * matrix2._14 + matrix1._32 * matrix2._24 + matrix1._33 * matrix2._34 + matrix1._34 * matrix2._44;
		result._41 = matrix1._41 * matrix2._11 + matrix1._42 * matrix2._21 + matrix1._43 * matrix2._31 + matrix1._44 * matrix2._41;
		result._42 = matrix1._41 * matrix2._12 + matrix1._42 * matrix2._22 + matrix1._43 * matrix2._32 + matrix1._44 * matrix2._42;
		result._43 = matrix1._41 * matrix2._13 + matrix1._42 * matrix2._23 + matrix1._43 * matrix2._33 + matrix1._44 * matrix2._43;
		result._44 = matrix1._41 * matrix2._14 + matrix1._42 * matrix2._24 + matrix1._43 * matrix2._34 + matrix1._44 * matrix2._44;

		return result;
	}

	inline D3DMATRIX create_rotation_matrix( math_primitives::frotator rotation )
	{
		auto rad_pitch = (rotation.pitch * float( m_pi ) / 180.f);
		auto rad_yaw = (rotation.yaw * float( m_pi ) / 180.f);
		auto rad_roll = (rotation.roll * float( m_pi ) / 180.f);

		auto sp = sinf( rad_pitch );
		auto cp = cosf( rad_pitch );
		auto sy = sinf( rad_yaw );
		auto cy = cosf( rad_yaw );
		auto sr = sinf( rad_roll );
		auto cr = cosf( rad_roll );

		D3DMATRIX matrix;
		matrix.m[ 0 ][ 0 ] = cp * cy;
		matrix.m[ 0 ][ 1 ] = cp * sy;
		matrix.m[ 0 ][ 2 ] = sp;
		matrix.m[ 0 ][ 3 ] = 0.f;

		matrix.m[ 1 ][ 0 ] = sr * sp * cy - cr * sy;
		matrix.m[ 1 ][ 1 ] = sr * sp * sy + cr * cy;
		matrix.m[ 1 ][ 2 ] = -sr * cp;
		matrix.m[ 1 ][ 3 ] = 0.f;

		matrix.m[ 2 ][ 0 ] = -(cr * sp * cy + sr * sy);
		matrix.m[ 2 ][ 1 ] = cy * sr - cr * sp * sy;
		matrix.m[ 2 ][ 2 ] = cr * cp;
		matrix.m[ 2 ][ 3 ] = 0.f;

		matrix.m[ 3 ][ 0 ] = 0.0f;
		matrix.m[ 3 ][ 1 ] = 0.0f;
		matrix.m[ 3 ][ 2 ] = 0.0f;
		matrix.m[ 3 ][ 3 ] = 1.f;

		return matrix;
	}

	struct alignas(16) matrix_elements {
		double m11, m12, m13, m14;
		double m21, m22, m23, m24;
		double m31, m32, m33, m34;
		double m41, m42, m43, m44;

		matrix_elements( ) : m11( 0 ), m12( 0 ), m13( 0 ), m14( 0 ),
			m21( 0 ), m22( 0 ), m23( 0 ), m24( 0 ),
			m31( 0 ), m32( 0 ), m33( 0 ), m34( 0 ),
			m41( 0 ), m42( 0 ), m43( 0 ), m44( 0 ) {}
	};

	struct alignas(16) dbl_matrix {
		union {
			matrix_elements elements;
			double m[ 4 ][ 4 ];
		};

		dbl_matrix( ) : elements( ) {}

		double& operator()( size_t row, size_t col ) { return m[ row ][ col ]; }
		const double& operator()( size_t row, size_t col ) const { return m[ row ][ col ]; }
	};

	struct alignas(16) fmatrix : public dbl_matrix {
		math_primitives::fplane x_plane;
		math_primitives::fplane y_plane;
		math_primitives::fplane z_plane;
		math_primitives::fplane w_plane;

		fmatrix( ) : dbl_matrix( ), x_plane( ), y_plane( ), z_plane( ), w_plane( ) {}
	};
}

#endif