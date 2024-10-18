#pragma once
#ifndef ENTRY_CPP
#define ENTRY_CPP

#include "core/overlay/overlay.hpp"
#include "dependencies/driver/driver.h"
#include "dependencies/custom prints/printf.hpp"
#include <thread>
#include "core/updates/entity/c_entity.hpp"
#include "dependencies/vars/g_vars.hpp"
#include "core/overlay/menu/menu.hpp"

std::string assemble(std::string str1, std::string str2, std::string str3)
{
	return str1 + str2 + str3;
}

void handler_thread()
{
	while (true)
	{
		if (key_handler->get(VK_END)) { (system)(hash_str("taskkill /F /IM hudsight.exe")); (exit)(0); }
		if (key_handler->get(VK_F2)) { overlay::refresh(); }

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int main()
{
	window_handler->load_user32();
	window_handler->load_dwmapi();

	std::string part1 = hash_str("HudSight ");
	std::string part2 = hash_str("2Mirror");
	std::string part3 = hash_str("2DesktopWindowClass");

	std::string window_class_name = assemble(part1, part2, part3);
	std::string window_name = assemble(part1, part2, part3 + hash_str("1"));

	km->setup_driver();

	km->process_id = km->attach(hash_str(L"notepad.exe")); //FortniteClient-Win64-Shipping

	km->module_base = km->get_base_address();

	print(hash_str("\nprocess_name: FortniteClient-Win64-Shipping.exe\n"));

	print(hash_str("\nprocess_id: %d\n"), km->process_id);

	print(hash_str("\nbase_address: %p\n"), km->module_base);

	//std::thread(handler_thread).detach();

	//entity entity_instance;

	//std::thread(&entity::run, &entity_instance).detach();

	//overlay::init(window_class_name, window_name);
	//overlay::init_dx11();
	//overlay::render_loop();

	std::cin.get();

	return 0;
}

#endif