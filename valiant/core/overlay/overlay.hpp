#pragma once
#include <utility>
#include <cstddef>
#include <intrin.h>
#include <string>
#include <utility>
#include <d3d11.h>
#include <dwmapi.h>
#include <iostream>
#include "../../dependencies/imgui/backend/imgui_impl_dx11.h"
#include "../../dependencies/imgui/backend/imgui_impl_win32.h"
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_internal.h"
#include "../../core/overlay/imports/calls.hpp"

inline HWND window;
inline ID3D11Device* d3d_device;
inline ID3D11DeviceContext* d3d_device_ctx;
inline IDXGISwapChain* d3d_swap_chain;
inline ID3D11RenderTargetView* d3d_render_target;
inline MARGINS window_margin = { -1 };
inline bool use_medal = false;

static class overlay
{
public:
    static bool init(std::string window_class, std::string window_);
    static void refresh();
    static void render();
    static bool render_loop();
    static bool init_dx11();
};
