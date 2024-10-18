#ifndef U_ENTITY_CPP
#define U_ENTITY_CPP

#include "../../../core/includes.hpp"
#include "../../../core/sdk/utils/n_utils.hpp"

#include "../entity.hpp"
#include "../world.hpp"

void entity::run()
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	std::vector<entity_data> s_entity_data_temp;

	while (true)
	{
		s_world_data->gengine_offset = utils::other::find_signature(hash_str("\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\xD0\x48\x8B\x00\xE8"), hash_str("xxx????xxxxx?x")) - km->module_base;

		std::cout << "g_engine: " << std::hex << s_world_data->gengine_offset << std::endl;

		s_world_data->gworld = utils::other::get_world( s_world_data->gengine_offset );
		if (!s_world_data->gworld) continue;

		std::cout << "g_world: " << std::dec << s_world_data->gworld << std::endl;

		s_world_data->game_state = km->read<uintptr_t>(s_world_data->gworld + offsets->GameState);
		if (!s_world_data->game_state) continue;

		std::cout << "game_state: " << std::dec << s_world_data->game_state << std::endl;

		s_world_data->game_instance = km->read<uintptr_t>(s_world_data->gworld + offsets->GameInstance);
		if (!s_world_data->game_instance) continue;

		s_world_data->local_player = km->read<uintptr_t>(km->read<uintptr_t>(s_world_data->game_instance + offsets->LocalPlayers));
		if (!s_world_data->local_player) continue;

		s_world_data->player_controller = km->read<uintptr_t>(s_world_data->local_player + offsets->PlayerController);
		if (!s_world_data->player_controller) continue;

		s_world_data->local_pawn = km->read<uintptr_t>(s_world_data->player_controller + offsets->AcknowledgedPawn);

		auto current_weapon = km->read<uintptr_t>(s_world_data->local_pawn + offsets->CurrentWeapon);

		utils::weapon::speed = km->read<float>(current_weapon + 0x1a78);
		utils::weapon::gravity = km->read<float>(current_weapon + 0x1ce4);

		if (utils::weapon::gravity < 0 || utils::weapon::speed < 0) {
			utils::weapon::gravity = 0;
			utils::weapon::speed = 0;
		}

		s_world_data->player_array_size = km->read<int>(s_world_data->game_state + offsets->PlayerArray + sizeof(uintptr_t));
		if (s_world_data->player_array_size == 0) continue;

		s_world_data->player_array = km->read<uintptr_t>(s_world_data->game_state + offsets->PlayerArray);
		std::unique_ptr<uintptr_t[]> player_state_array = km->read_array(s_world_data->player_array, s_world_data->player_array_size);

		for (int i = 0; i < s_world_data->player_array_size; ++i)
		{
			entity_data cached;
			auto temp_player_state = player_state_array[i];
			if (!temp_player_state) continue;

			auto temp_pawn_private = km->read<uintptr_t>(temp_player_state + offsets->PawnPrivate);
			if (!temp_pawn_private) continue;

			auto temp_mesh = km->read<uintptr_t>(temp_pawn_private + offsets->Mesh);
			if (!temp_mesh) continue;

			cached.player_state = temp_player_state;
			cached.pawn_private = temp_pawn_private;
			cached.mesh = temp_mesh;

			s_entity_data_temp.push_back(cached);
		}

		s_entity_data = std::move(s_entity_data_temp);
		s_entity_data_temp.clear();

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

#endif