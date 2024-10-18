#include "../../../core/overlay/menu/menu.hpp"
#include "../../../dependencies/vars/g_vars.hpp"
#include <vector>
#include "../../../dependencies/cipher/hash.hpp"

bool combat = false;
bool triggerbot = false;
bool visuals = false;
bool settings = false;

void menu::draw()
{
    ImGui::Begin(hash_str("Valiant - Private"));

    if (ImGui::BeginTabBar(hash_str("##tabs")))
    {
        if (ImGui::BeginTabItem("Combat"))
        {
            ImGui::Checkbox(hash_str("Enable Combat"), &g_vars->aimbot.enabled);

            ImGui::Checkbox(hash_str("Enable Radius"), &g_vars->aimbot.display.fov);

            ImGui::SliderFloat(hash_str("Radius Slider"), &g_vars->aimbot.fov, 0, 250);

            ImGui::SliderFloat(hash_str("Smoothing Slider"), &g_vars->aimbot.smoothing, 0, 10);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(hash_str("Triggerbot")))
        {
            ImGui::Checkbox(hash_str("Enable Triggerbot"), &g_vars->aimbot.triggerbot.enabled);

            ImGui::SliderFloat(hash_str("Millisecond Delay"), &g_vars->aimbot.triggerbot.ms_delay, 0, 500);

            ImGui::SliderFloat(hash_str("Maximum Distance"), &g_vars->aimbot.triggerbot.max_distance, 0, 15);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(hash_str("Visuals")))
        {
            ImGui::Checkbox(hash_str("Enable Box ESP"), &g_vars->visuals.box);

            ImGui::SliderInt(hash_str("Box Thickness"), &g_vars->visuals.box_thickness, 0, 5);

            ImGui::Checkbox(hash_str("Enable Skeleton ESP"), &g_vars->visuals.skeleton);

            ImGui::Checkbox(hash_str("Enable Username ESP"), &g_vars->visuals.name);

            ImGui::Checkbox(hash_str("Enable Rank ESP"), &g_vars->visuals.rank);

            ImGui::Checkbox(hash_str("Enable Weapon-Class ESP"), &g_vars->visuals.weapon_class);

            ImGui::Checkbox(hash_str("Enable Ammo-Count ESP"), &g_vars->visuals.ammo);

            ImGui::SliderFloat(hash_str("Maximum Distance"), &g_vars->visuals.max_distance, 0, 300);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(hash_str("Settings")))
        {
            ImGui::Checkbox(hash_str("Enable VSync"), &g_vars->misc.vsync);

            ImGui::Checkbox(hash_str("Enable Valiant Watermark"), &g_vars->misc.watermark.logo);

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void menu::set_style()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Modify ImGui style colors
    ImVec4* colors = style.Colors;
    const ImVec4 black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    const ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    style.WindowPadding = ImVec2(8, 8);
    style.WindowRounding = 2.0f;
    style.FramePadding = ImVec2(4, 3);
    style.FrameRounding = 2.0f;
    style.ItemSpacing = ImVec2(8, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.IndentSpacing = 21.0f;
    style.ScrollbarSize = 16.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabMinSize = 10.0f;
    style.GrabRounding = 2.0f;

    // Background
    colors[ImGuiCol_WindowBg] = black;

    // Text
    colors[ImGuiCol_Text] = white;

    // Widgets
    colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.24f, 0.24f, 0.24f, 1.0f);
    colors[ImGuiCol_Separator] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);

    // Frame
    colors[ImGuiCol_Border] = white;

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);

    // Menu
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);

    // Rounding
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;

    // Sizes/spacing
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(5, 5);
    style.ItemSpacing = ImVec2(10, 5);

    // Border
    style.WindowBorderSize = 1.0f;

    // Apply the modified style
    ImGui::StyleColorsDark();
}