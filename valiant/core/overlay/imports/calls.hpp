#pragma once
#include <windows.h>
#include <dwmapi.h>
#include <iostream>
#include "../../../dependencies/cipher/callstack.hpp"
#include "../../../dependencies/cipher/hash.hpp"

class key_state_handler
{
public:
    void load_user32() 
    {
        hide;
        HMODULE user32 = hide_call(LoadLibraryA)(hash_str("user32.dll"));
        if (!user32) 
        {
            return;
        }

        void* GetAsyncKeyStateProc = (void*)GetProcAddress(user32, hash_str("GetAsyncKeyState"));
        if (!GetAsyncKeyStateProc) 
        {
            hide_call(FreeLibrary)(user32);
            return;
        }

        void* keybdEventProc = (void*)GetProcAddress(user32, hash_str("keybd_event"));
        if (!keybdEventProc)
        {
            hide_call(FreeLibrary)(user32);
            return;
        }

        *(void**)&_GetAsyncKeyState = GetAsyncKeyStateProc;
        *(void**)&_keybdEvent = keybdEventProc;
        hide_call(FreeLibrary)(user32);
    }

    SHORT get(int vKey) 
    {
        hide;
        if (_GetAsyncKeyState != nullptr)
        {
            return _GetAsyncKeyState(vKey);
        }
        return 0;
    }

    void key_event(BYTE bVk, BYTE bScan, DWORD dwFlags, ULONG_PTR dwExtraInfo)
    {
        hide;
        if (_keybdEvent != nullptr)
        {
            _keybdEvent(bVk, bScan, dwFlags, dwExtraInfo);
        }
    }

private:
    SHORT(*_GetAsyncKeyState)(int vKey);
    void (*_keybdEvent)(BYTE bVk, BYTE bScan, DWORD dwFlags, ULONG_PTR dwExtraInfo);
};

inline std::unique_ptr<key_state_handler> key_handler = std::make_unique<key_state_handler>();

class window_handler_ 
{
public:

    void load_user32() 
    {
        hide;
        user32 = hide_call(LoadLibraryA)(hash_str("user32.dll"));
        if (!user32)
        {
            return;
        }

        void* procAddr = (void*)GetProcAddress(user32, hash_str("FindWindowA"));
        if (procAddr) 
        {
            *(void**)&_FindWindow = procAddr;
        }

        procAddr = (void*)GetProcAddress(user32, hash_str("SetWindowPos"));
        if (procAddr) 
        {
            *(void**)&_SetWindowPos = procAddr;
        }

        procAddr = (void*)GetProcAddress(user32, hash_str("SetLayeredWindowAttributes"));
        if (procAddr)
        {
            *(void**)&_SetLayeredWindowAttributes = procAddr;
        }

        procAddr = (void*)GetProcAddress(user32, hash_str("UpdateWindow"));
        if (procAddr)
        {
            *(void**)&_UpdateWindow = procAddr;
        }
    }

    void load_dwmapi()
    {
        hide;
        dwmapi = hide_call(LoadLibraryA)(hash_str("dwmapi.dll"));
        if (!dwmapi) 
        {
            return;
        }

        void* procAddr = (void*)GetProcAddress(dwmapi, hash_str("DwmExtendFrameIntoClientArea"));
        if (procAddr) 
        {
            *(void**)&_DwmExtendFrameIntoClientArea = procAddr;
        }
    }

    HWND find(const std::string& className, const std::string& windowName)
    {
        hide;
        if (_FindWindow != nullptr) 
        {
            return _FindWindow(className.c_str(), windowName.c_str());
        }
        return nullptr;
    }

    bool set(HWND hwnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
    {
        hide;
        if (_SetWindowPos != nullptr)
        {
            return _SetWindowPos(hwnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
        }
        return false;
    }

    bool set_layered(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags) 
    {
        hide;
        if (_SetLayeredWindowAttributes != nullptr) 
        {
            return _SetLayeredWindowAttributes(hwnd, crKey, bAlpha, dwFlags);
        }
        return false;
    }

    HRESULT extend_frame(HWND hwnd, const MARGINS* pMargins)
    {
        hide;
        if (_DwmExtendFrameIntoClientArea != nullptr) 
        {
            return _DwmExtendFrameIntoClientArea(hwnd, pMargins);
        }
        return E_FAIL;
    }

    bool update(HWND hwnd)
    {
        hide;
        if (_UpdateWindow != nullptr)
        {
            return _UpdateWindow(hwnd);
        }
        return false;
    }

private:
    HMODULE user32 = nullptr;
    HMODULE dwmapi = nullptr;
    HWND(WINAPI* _FindWindow)(LPCSTR, LPCSTR) = nullptr;
    BOOL(WINAPI* _SetWindowPos)(HWND, HWND, int, int, int, int, UINT) = nullptr;
    BOOL(WINAPI* _SetLayeredWindowAttributes)(HWND, COLORREF, BYTE, DWORD) = nullptr;
    HRESULT(WINAPI* _DwmExtendFrameIntoClientArea)(HWND, const MARGINS*) = nullptr;
    BOOL(WINAPI* _UpdateWindow)(HWND) = nullptr;
};

inline std::unique_ptr<window_handler_> window_handler = std::make_unique<window_handler_>();