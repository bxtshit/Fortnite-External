#pragma once
#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>
#include <iostream>
#include "../../dependencies/driver/driver_requests.h"
#include <vector>

class km_f
{
public:
	uintptr_t module_base;
	HANDLE device_handle;
	INT32 process_id;

	bool setup_driver();

	uintptr_t attach(LPCTSTR process_name);

	uintptr_t get_base_address();

	bool get_dtb();


	bool read_physical(PVOID address, PVOID buffer, DWORD size);

	template <typename T>
	T read(uint64_t address)
	{
		T buffer{ };
		km_f::read_physical((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}

	template <typename T>
	T special_read(uint64_t address)
	{
		T buffer{ };
		read_physical((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}

	std::unique_ptr<uintptr_t[]> read_array(uintptr_t address, int count)
	{
		std::vector<uintptr_t> value(count);

		if (count > 0)
		{
			read_physical((PVOID)address, static_cast<void*>(value.data()), sizeof(uintptr_t) * count);
		}

		std::unique_ptr<uintptr_t[]> result(new uintptr_t[count]);
		std::copy(value.begin(), value.end(), result.get());
		return result;
	}

	bool read_array_real(std::uintptr_t address, void* buffer, std::size_t size) 
	{
		return read_physical(reinterpret_cast<PVOID>(address), buffer, static_cast<DWORD>(size));
	}
};

inline std::unique_ptr<km_f> km = std::make_unique<km_f>();