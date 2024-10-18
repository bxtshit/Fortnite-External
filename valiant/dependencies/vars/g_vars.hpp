#pragma once
#include <locale>

class vars
{
public:
	struct
	{
		bool enabled = false;

		int type = 0;

		float fov = 250;

		float smoothing = 5;

		struct
		{
			bool dot = false;
			bool line = false;
			bool fov = false;
		} display;

		struct
		{
			bool enabled = false;
			float ms_delay = 100;
			float max_distance = 10;
		}triggerbot;

	} aimbot;

	struct
	{
		bool box = false;
		int box_thickness = 1.5;
		bool skeleton = false;
		bool distance = false;
		bool weapon = false;
		bool ammo = false;
		bool rank = false;
		bool name = false;
		bool weapon_class = false;

		float max_distance = 300;
	} visuals;

	struct
	{
		bool vsync = false;

		struct
		{
			bool logo = true;
			bool logo_2 = true;
			bool tps = false;
		} watermark;
	} misc;
}; inline std::unique_ptr<vars> g_vars = std::make_unique<vars>();