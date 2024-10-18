#include "../../dependencies/driver/driver.h"
#include "../cipher/hash.hpp"
#include <unordered_map>

bool km_f::setup_driver()
{
    device_handle = CreateFileW(hash_str(L"\\\\.\\{9A3B89F7-6C4D-4E18-BD62-DA84FC7A3D68}"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

    if (!device_handle || (device_handle == INVALID_HANDLE_VALUE))
    {
        printf("[+] Handle Not Found.");
        return false;
    }

    return true;
}

uintptr_t km_f::attach(LPCTSTR process_name)
{
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt)) {
        do {
            if (!lstrcmpi(pt.szExeFile, process_name))
            {
                CloseHandle(hsnap);
                km->process_id = pt.th32ProcessID;
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
    }
    CloseHandle(hsnap);

    return { NULL };
}

uintptr_t km_f::get_base_address()
{
    uintptr_t image_address = { NULL };
    BaseAddress data = { NULL };

    data.Security = CODE_SECURITY;
    data.ProcessId = process_id;
    data.Address = (ULONGLONG*)&image_address;

    DeviceIoControl(device_handle, IOCTL_GET_BASE_ADDRESS, &data, sizeof(data), nullptr, NULL, NULL, NULL);

    return image_address;
}

bool km_f::read_physical(PVOID address, PVOID buffer, DWORD size)
{
    ReadWrite data = { 0 };

    data.Security = CODE_SECURITY;
    data.Address = (ULONGLONG)address;
    data.Buffer = (ULONGLONG)buffer;
    data.Size = size;
    data.ProcessId = process_id;
    data.Write = FALSE;

    return DeviceIoControl(device_handle, IOCTL_READWRITE, &data, sizeof(data), nullptr, NULL, NULL, NULL);
}