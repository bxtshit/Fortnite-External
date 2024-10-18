#ifndef N_UTILS_CPP
#define N_UTILS_CPP

#include "../../../core/sdk/utils/n_utils.hpp"
#include <memory>  
#include <cstdint>  
#include <array> 


std::pair<uintptr_t, uengine::ftransform> utils::bone::get_bone_data( uintptr_t mesh )
{
	auto bone_array = km->read<uintptr_t>( mesh + offsets->BoneArray + (km->read<int>( mesh + offsets->BoneCached ) * 0x10) );
	auto component_to_world = km->read<uengine::ftransform>( mesh + offsets->ComponentToWorld );

	return std::make_pair( bone_array, component_to_world );
}

uengine::ftransform utils::bone::get_bone_transform( uintptr_t bone_array, int id )
{
	return km->read<uengine::ftransform>( bone_array + id * 0x60 );
}

math_primitives::fvector utils::bone::process_bone_transform( uengine::ftransform bone_transform, uengine::ftransform component_to_world )
{
	auto bone_matrix = uengine::matrix_multiplication( bone_transform.to_matrix_with_scale( ), component_to_world.to_matrix_with_scale( ) );
	return math_primitives::fvector( bone_matrix._41, bone_matrix._42, bone_matrix._43 );
}

void utils::camera::setup( )
{
	const auto gworld = km->read<uintptr_t>( km->module_base + offsets->GWorld );
	const auto game_instance = km->read<uintptr_t>( gworld + offsets->GameInstance );
	const auto local_player = km->read<uintptr_t>( km->read<uintptr_t>( game_instance + offsets->LocalPlayers ) );
	const auto view_matrix = km->read<uintptr_t>( local_player + 0xd0 );

	view_state = km->read<uintptr_t>( view_matrix + 0x8 );
}

void utils::camera::update( )
{
	auto projection = km->read<uengine::fmatrix>( view_state + 0x900 );

	camera_data->rotation.pitch = asin( projection.z_plane.w ) * rad_to_deg;
	camera_data->rotation.yaw = atan2( projection.y_plane.w, projection.x_plane.w ) * rad_to_deg;
	camera_data->rotation.roll = 0.0f;

	camera_data->location.x = projection.m[ 3 ][ 0 ];
	camera_data->location.y = projection.m[ 3 ][ 1 ];
	camera_data->location.z = projection.m[ 3 ][ 2 ];

	auto fov_radians = 2.0f * atanf( 1.0f / static_cast<float>(km->read<double>( view_state + 0x700 )) );
	camera_data->fov = fov_radians * rad_to_deg;
}

math_primitives::fvector2d utils::screen::world_to_screen( math_primitives::fvector location, D3DMATRIX rotation_matrix, float fov_type_shit )
{
	auto& axis_x = rotation_matrix.m[ 0 ];
	auto& axis_y = rotation_matrix.m[ 1 ];
	auto& axis_z = rotation_matrix.m[ 2 ];

	math_primitives::fvector delta = location - camera_data->location;

	auto transformed_x = delta.dot( math_primitives::fvector( axis_y[ 0 ], axis_y[ 1 ], axis_y[ 2 ] ) );
	auto transformed_y = delta.dot( math_primitives::fvector( axis_z[ 0 ], axis_z[ 1 ], axis_z[ 2 ] ) );
	auto transformed_z = max( delta.dot( math_primitives::fvector( axis_x[ 0 ], axis_x[ 1 ], axis_x[ 2 ] ) ), 1.0f );

	auto screen_x = math_primitives::s_width_center + transformed_x * (math_primitives::s_width_center / fov_type_shit) / transformed_z;
	auto screen_y = math_primitives::s_height_center - transformed_y * (math_primitives::s_width_center / fov_type_shit) / transformed_z;

	return math_primitives::fvector2d( screen_x, screen_y );
}

std::string utils::weapon::get_weapon_name( uintptr_t current_weapon )
{
	auto weapon_data = km->read<uint64_t>( current_weapon + offsets->WeaponData );
	if ( !weapon_data ) return hash_str( "null" );

	auto fname_text = km->read<uint64_t>( weapon_data + offsets->ItemName );
	if ( !fname_text ) return hash_str( "null" );

	auto name_length = km->read<uint32_t>( fname_text + 0x30 );
	if ( !name_length ) return hash_str( "null" );

	wchar_t* name = new wchar_t[ uint64_t( name_length ) + 1 ];

	//if ( !vm->read_array( (uintptr_t)vm->read<PVOID>( fname_text + 0x28 ), (name), name_length * sizeof( wchar_t ) ) ) { log_error( ); }

	std::wstring wname( name );
	return std::string( wname.begin( ), wname.end( ) );
}

math_primitives::fvector utils::weapon::predict_location( const math_primitives::fvector& target_initial, const math_primitives::fvector& velocity, float distance )
{
	auto target = target_initial;
	auto time = distance / speed;

	target.add_scale( velocity, time );

	auto gravity1 = fabs( -980.0f * gravity ) * 0.5f * time * time;
	target.z += gravity1;

	return target;
}

bool utils::other::is_visible( uintptr_t mesh )
{
	auto submit_time = km->read<float>( mesh + 0x2E8 );
	auto render_time = km->read<float>( mesh + 0x2F0 );

	return render_time + 0.06f >= submit_time;
}

const auto ServerTime = km->read<float>(offsets->GameState + 0xC38);

std::string utils::other::decrypt_name(uintptr_t state)
{
	__int64 ftext;
	int length;

	if (!ServerTime)
	{
		length = km->read<int>(state + 0x330);

		ftext = km->read<__int64>(state + 0x328);
	}
	else
	{
		auto fstring = km->read<__int64>(state + 0xAB0);

		length = km->read<int>(fstring + 0x10);

		ftext = (uintptr_t)km->read<__int64>(fstring + 0x8);
	}

	if (length > 0 && length < 50)
	{
		wchar_t* buffer = new wchar_t[length];
		km->read_physical((PVOID)ftext, buffer, length * sizeof(wchar_t));

		auto v6 = (__int64)length;

		char v21;
		int v22;
		int i;
		int v25;
		WORD* v23;

		v21 = v6 - 1;
		if (!(WORD)v6)
			v21 = 0;
		v22 = 0;
		v23 = (WORD*)buffer;
		for (i = (v21) & 3; ; *v23++ += i & 7)
		{
			v25 = v6 - 1;
			if (!(WORD)v6)
				v25 = 0;
			if (v22 >= v25)
				break;
			i += 3;
			++v22;
		}

		std::wstring username{ buffer };

		delete[] buffer;

		return std::string(username.begin(), username.end());
	}
}

auto utils::other::find_signature(const char* sig, const char* mask) -> std::uintptr_t
{
	if (!km->module_base)
		return { };

	auto buffer = std::make_unique<std::array<std::uint8_t, 0x100000>>();
	auto data = buffer.get()->data();

	for (std::uintptr_t i = 0u; i < (2u << 25u); ++i)
	{
		if (!km->read_array_real(km->module_base + i * 0x100000, data, 0x100000))
		{
			printf("\nfailed to read\n");
		}

		if (!data) {
			return 0;
		}

		for (std::uintptr_t j = 0; j < 0x100000u; ++j)
		{
			if ([](std::uint8_t const* data, std::uint8_t const* sig, char const* mask)
				{
					for (; *mask; ++mask, ++data, ++sig)
					{
						if (*mask == 'x' && *data != *sig) return false;
					}
					return (*mask) == 0;
				}(data + j, (std::uint8_t*)sig, mask))
			{

				std::uintptr_t result = km->module_base + i * 0x100000 + j;
				std::uint32_t rel = 0;

				if (!km->read_array_real(result + 3, &rel, sizeof(std::uint32_t)))
				{
					printf("\nfailed to read #2\n");
				}

				if (!rel)
					return { };

				return result + rel + 7;
			}
		}
	}

	return 0;
}

std::uintptr_t utils::other::get_world(uintptr_t gengine_offset)
{
	auto gengine = km->read<uintptr_t>(km->module_base + gengine_offset);
	if (!gengine)
		return { };

	auto viewport = km->read<uintptr_t>(gengine + 0x910);
	if (!viewport)
		return { };

	return km->read<std::uintptr_t>(viewport + 0x78);
}

std::string utils::other::get_rank( int32_t rank_progress )
{
	if ( rank_progress == 0 )
		return std::string( hash_str( "Bronze 1" ) );
	else if ( rank_progress == 1 )
		return std::string( hash_str( "Bronze 2" ) );
	else if ( rank_progress == 2 )
		return std::string( hash_str( "Bronze 3" ) );
	else if ( rank_progress == 3 )
		return std::string( hash_str( "Silver 1" ) );
	else if ( rank_progress == 4 )
		return std::string( hash_str( "Silver 2" ) );
	else if ( rank_progress == 5 )
		return std::string( hash_str( "Silver 3" ) );
	else if ( rank_progress == 6 )
		return std::string( hash_str( "Gold 1" ) );
	else if ( rank_progress == 7 )
		return std::string( hash_str( "Gold 2" ) );
	else if ( rank_progress == 8 )
		return std::string( hash_str( "Gold 3" ) );
	else if ( rank_progress == 9 )
		return std::string( hash_str( "Platinum 1" ) );
	else if ( rank_progress == 10 )
		return std::string( hash_str( "Platinum 2" ) );
	else if ( rank_progress == 11 )
		return std::string( hash_str( "Platinum 3" ) );
	else if ( rank_progress == 12 )
		return std::string( hash_str( "Platinum 1" ) );
	else if ( rank_progress == 13 )
		return std::string( hash_str( "Platinum 2" ) );
	else if ( rank_progress == 14 )
		return std::string( hash_str( "Platinum 3" ) );
	else if ( rank_progress == 15 )
		return std::string( hash_str( "Elite" ) );
	else if ( rank_progress == 16 )
		return std::string( hash_str( "Champion" ) );
	else if ( rank_progress == 17 )
		return std::string( hash_str( "Unreal" ) );
	else
		return std::string( hash_str( "Null" ) );
}

#endif