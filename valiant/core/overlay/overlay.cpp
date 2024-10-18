#include <thread>
#include "overlay.hpp"
#include "../../dependencies/vars/g_vars.hpp"
#include "../../core/overlay/menu/menu.hpp"
#include "../render/c_render.hpp"
#include "../../core/overlay/menu/fonts/exported.hpp"

struct {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    int width_center = width / 2;
    int height_center = height / 2;
} inline monitor;

bool overlay::init(std::string window_class, std::string window_)
{
    hide;
    window = window_handler->find(window_class.c_str(), window_.c_str());
    if (!window) { return false; }

    window_handler->extend_frame(window, &window_margin);
    window_handler->set_layered(window, 255, 255, LWA_ALPHA);
    window_handler->set(window, 0, 0, 0, monitor.width, monitor.height, 0);
    window_handler->update(window);

    return true;
}

void overlay::refresh()
{
    hide;
    window_handler->set(window, 0, 0, 0, monitor.width, monitor.height, SWP_SHOWWINDOW);
}

bool show_menu = true;

const ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

const float clear_color_with_alpha[4] = {
    clear_color.x * clear_color.w,
    clear_color.y * clear_color.w,
    clear_color.z * clear_color.w,
    clear_color.w
};

render render_instance;

void overlay::render()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (GetAsyncKeyState(VK_INSERT) & 1)
    {
        menu_ptr.toggle();
    }

    render_instance.run();

    menu_ptr.draw();

    ImGui::Render();

    d3d_device_ctx->OMSetRenderTargets(1, &d3d_render_target, nullptr);
    d3d_device_ctx->ClearRenderTargetView(d3d_render_target, clear_color_with_alpha);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    d3d_swap_chain->Present(g_vars->misc.vsync ? 1 : 0, 0);
}

bool overlay::render_loop()
{
    MSG msg = { 0 };

    if (!IsWindow(window))
    {
        printf(hash_str("\nerror: invalid_hwnd\n"));
        return false;
    }

    while (true)
    {
        if (PeekMessage(&msg, window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }

        ImGuiIO& io = ImGui::GetIO();

        io.DeltaTime = 1.0f / 60.0f;

        POINT p_cursor;
        if (!GetCursorPos(&p_cursor))
        {
            printf("\nerror: failed to get cursor_position\n");
            continue; 
        }
        ScreenToClient(window, &p_cursor);
        io.MousePos.x = p_cursor.x;
        io.MousePos.y = p_cursor.y;

        io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        io.MouseClicked[0] = io.MouseDown[0];

        overlay::render(); 
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    DestroyWindow(window);

    return true;
}

void cleanup_dx11()
{
    if (d3d_device_ctx) { d3d_device_ctx->Release(); d3d_device_ctx = nullptr; }
    if (d3d_render_target) { d3d_render_target->Release(); d3d_render_target = nullptr; }
    if (d3d_swap_chain) { d3d_swap_chain->Release(); d3d_swap_chain = nullptr; }
    if (d3d_device) { d3d_device->Release(); d3d_device = nullptr; }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool overlay::init_dx11()
{
    DXGI_SWAP_CHAIN_DESC swap_chain_description;
    ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
    swap_chain_description.BufferCount = 2;
    swap_chain_description.BufferDesc.Width = 0;
    swap_chain_description.BufferDesc.Height = 0;
    swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
    swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_description.OutputWindow = window;
    swap_chain_description.SampleDesc.Count = 1;
    swap_chain_description.SampleDesc.Quality = 0;
    swap_chain_description.Windowed = 1;
    swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const D3D_FEATURE_LEVEL d3d_feature_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

    D3D_FEATURE_LEVEL d3d_feature_lvl;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        d3d_feature_array,
        2,
        D3D11_SDK_VERSION,
        &swap_chain_description,
        &d3d_swap_chain,
        &d3d_device,
        &d3d_feature_lvl,
        &d3d_device_ctx
    );

    if (FAILED(hr)) {
        return false;
    }

    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = d3d_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (FAILED(hr)) {
        cleanup_dx11();
        return false;
    }

    hr = d3d_device->CreateRenderTargetView(pBackBuffer, nullptr, &d3d_render_target);
    pBackBuffer->Release();
    if (FAILED(hr)) {
        cleanup_dx11();
        return false;
    }

    if (!ImGui::CreateContext()) {
        cleanup_dx11();
        return false;
    }

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (!ImGui_ImplWin32_Init(window) || !ImGui_ImplDX11_Init(d3d_device, d3d_device_ctx)) {
        cleanup_dx11();
        return false;
    }

    io.Fonts->AddFontFromMemoryTTF(inter_light, sizeof(inter_light), 16, nullptr, io.Fonts->GetGlyphRangesCyrillic());

    font::regular = io.Fonts->AddFontFromMemoryTTF(inter_light, sizeof(inter_light), 16, nullptr, io.Fonts->GetGlyphRangesCyrillic());

    return true;
} 