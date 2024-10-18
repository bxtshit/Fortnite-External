#pragma once
#ifndef N_UTILS_HPP
#define N_UTILS_HPP

#include "../../../core/includes.hpp"

namespace utils
{
	namespace camera
	{
		void setup( );
		void update( );
		inline uintptr_t view_state = null;
		inline const float rad_to_deg = 180.0f / m_pi;
	}

	namespace bone
	{
		std::pair<uintptr_t, uengine::ftransform> get_bone_data( uintptr_t mesh );
		uengine::ftransform get_bone_transform( uintptr_t bone_array, int id );
		math_primitives::fvector process_bone_transform( uengine::ftransform bone_transform, uengine::ftransform component_to_world );
	}

	namespace screen
	{
		math_primitives::fvector2d world_to_screen( math_primitives::fvector location, D3DMATRIX rotation_matrix, float fov_type_shit );
	}

	namespace weapon
	{
		std::string get_weapon_name( uintptr_t current_weapon );
		math_primitives::fvector predict_location( const math_primitives::fvector& target_initial, const math_primitives::fvector& velocity, float distance );

		inline float speed;
		inline float gravity;
	}

	namespace other
	{
		bool is_visible( uintptr_t mesh );
		std::string decrypt_name(uintptr_t state);
		auto find_signature(const char* sig, const char* mask) -> std::uintptr_t;
		std::uintptr_t get_world(uintptr_t gengine_offset);

		std::string get_rank( int32_t rank_progress );
	}
}

class a_camera_data
{
public:
	math_primitives::fvector location;
	math_primitives::frotator rotation;
	float fov;
}; inline std::unique_ptr<a_camera_data> camera_data = std::make_unique<a_camera_data>( );

#endif