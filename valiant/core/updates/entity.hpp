#pragma once
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../includes.hpp"

struct entity_data
{
	uintptr_t player_state = null;
	uintptr_t pawn_private = null;
	uintptr_t mesh = null;
	uintptr_t bone_array;

	uengine::ftransform component_to_world;

	uengine::ftransform transform;

	char dying = null;
	char downed = null;
	bool visible = null;
}; inline std::vector<entity_data> s_entity_data;

#endif