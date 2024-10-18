#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP

#include "../includes.hpp"

struct world_data
{
	uintptr_t gworld = null;
	uintptr_t game_instance = null;
	uintptr_t game_state = null;
	uintptr_t local_player = null;
	uintptr_t local_pawn = null;
	uintptr_t gengine_offset = null;
	uintptr_t gworld_signature = null;
	uintptr_t player_controller = null;
	uintptr_t player_array = null;

	int player_array_size;
}; inline std::unique_ptr<world_data> s_world_data = std::make_unique<world_data>( );

#endif