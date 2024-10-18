#ifndef C_RENDER_CPP
#define C_RENDER_CPP

#include "../../core/updates/entity.hpp"
#include "../../dependencies/imgui/imgui.h"
#include "../../core/sdk/utils/n_utils.hpp"
#include "../../core/sdk/colors.hpp"
#include "../../dependencies/vars/g_vars.hpp"
#include "../../core/sdk/drawing.hpp"
#include "../overlay/menu/menu.hpp"

void render::run()
{
    const auto draw_list = ImGui::GetBackgroundDrawList();

    const colors color_list;

    auto closest_distance = DBL_MAX;
    auto closest_entity = entity_data(NULL);

    if (g_vars->misc.watermark.logo || g_vars->misc.watermark.logo_2)
    {
        int x = 5;
        int y = 5;

        if (g_vars->misc.watermark.logo)
        {
            ImU32 ocean_blue_color = ImGui::GetColorU32(ImVec4(30.0f / 255.0f, 144.0f / 255.0f, 255.0f / 255.0f, 1.0f));

            drawing.text(draw_list, x, y, ocean_blue_color, hash_str("Valiant"), true);
            ImVec2 text_size = ImGui::CalcTextSize(hash_str("Valiant"));
            x += text_size.x + 5;
        }

        if (g_vars->misc.watermark.logo_2)
        {
            drawing.text(draw_list, x, y, ImColor(0, 0, 0, 255), (hash_str("Private")), true);
        }
    }

    if (g_vars->aimbot.display.fov)
    {
        draw_list->AddCircle(ImVec2(math_primitives::s_width_center, math_primitives::s_height_center), g_vars->aimbot.fov, ImColor(255, 255, 255), 2);
    }

    utils::camera::update();

    auto rotation_matrix = uengine::create_rotation_matrix(camera_data->rotation);
    const auto aspect_ratio = static_cast<float>(math_primitives::s_width) / math_primitives::s_height;
    auto adjusted_fov = aspect_ratio / (16.0f / 9.0f) * tanf(camera_data->fov * static_cast<float>(m_pi) / 360.0f);

    for (auto& entity : s_entity_data)
    {
        auto pawn_private = entity.pawn_private;
        auto dying = (km->read<char>(pawn_private + offsets->bIsDying) >> 4) & 1;
        auto downed = (km->read<char>(pawn_private + offsets->bIsDBNO) >> 4) & 1;
        auto visible = utils::other::is_visible(entity.mesh);

        if (dying) continue;

        entity.dying = dying;
        entity.downed = downed;
        entity.visible = visible;

        auto main_colors = downed ? color_list.white_color : (!visible ? color_list.red_color : color_list.green_color); // if downed white, if not visible red, else green
        auto main_color_skeleton = downed ? color_list.white_color : (!visible ? color_list.red_color : color_list.green_color); // if downed white, if not visible red, else green

        std::pair<uintptr_t, uengine::ftransform> bone_data = utils::bone::get_bone_data(entity.mesh);

        uintptr_t bone_array = bone_data.first;
        uengine::ftransform component_to_world = bone_data.second;

        uengine::ftransform head_bone = utils::bone::get_bone_transform(bone_array, 110);

        uengine::ftransform neck_bone = utils::bone::get_bone_transform(bone_array, 68);

        uengine::ftransform root_bone = utils::bone::get_bone_transform(bone_array, 0);

        math_primitives::fvector head_bone_position = utils::bone::process_bone_transform(head_bone, component_to_world);

        math_primitives::fvector neck_bone_position = utils::bone::process_bone_transform(neck_bone, component_to_world);

        math_primitives::fvector root_bone_position = utils::bone::process_bone_transform(root_bone, component_to_world);

        auto distance = static_cast<int>(camera_data->location.distance(head_bone_position) / 100.f);

        if (distance > g_vars->visuals.max_distance) continue;

        math_primitives::fvector2d head_w2s = utils::screen::world_to_screen(head_bone_position, rotation_matrix, adjusted_fov);

        auto head_box = utils::screen::world_to_screen(math_primitives::fvector(head_bone_position.x, head_bone_position.y, head_bone_position.z + 15), rotation_matrix, adjusted_fov);

        math_primitives::fvector2d root_w2s = utils::screen::world_to_screen(root_bone_position, rotation_matrix, adjusted_fov);

        auto box_height = abs(head_box.y - root_w2s.y);
        auto box_width = box_height * 0.40;

        if (g_vars->visuals.box)
        {
			drawing.box_2d(draw_list, head_box.x - (box_width / 2), head_box.y, box_width, box_height, ImColor(main_colors), g_vars->visuals.box_thickness);
        }

        if (g_vars->visuals.distance)
        {
            std::string text = (hash_str("[")) + std::to_string(int(distance)) + (hash_str("m]"));
            ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
            drawing.text(draw_list, root_w2s.x - text_size.x / 2, root_w2s.y, color_list.white_color, text.c_str(), false);
        }

        if (g_vars->visuals.weapon)
        {
            auto current_weapon = km->read<uintptr_t>(entity.pawn_private + 0xA68);
            std::string weapon = utils::weapon::get_weapon_name(current_weapon);

            if (g_vars->visuals.ammo)
            {
                auto ammo_count = km->read<int32_t>(current_weapon + 0xEDC);
                weapon += (hash_str(" [")) + std::to_string(ammo_count) + (hash_str("]"));
            }

            ImVec2 text_size = ImGui::CalcTextSize(weapon.c_str());
            drawing.text(draw_list, head_box.x - text_size.x / 2, head_box.y - 15, color_list.white_color, weapon.c_str(), false);
        }

        if (g_vars->visuals.name)
        {
            int offset = g_vars->visuals.weapon ? 30 : 15;

            auto name = utils::other::decrypt_name(entity.player_state);
            ImVec2 text_size = ImGui::CalcTextSize(name.c_str());
            drawing.text(draw_list, head_box.x - text_size.x / 2, head_box.y - offset, color_list.white_color, name.c_str(), false);
        }

        if (g_vars->visuals.rank)
        {
            int offset = g_vars->visuals.distance ? 15 : 0;

            auto habanero_component = km->read<uintptr_t>(entity.player_state + 0x9F0);
            auto rank_progress = km->read<int32_t>(habanero_component + 0xB8 + 0x10);
            auto text = utils::other::get_rank(rank_progress);
            auto text_size = ImGui::CalcTextSize(text.c_str());
            drawing.text(draw_list, root_w2s.x - text_size.x / 2, root_w2s.y + offset, color_list.white_color, text.c_str(), false);
        }

        auto dx = head_w2s.x - math_primitives::s_width_center;
        auto dy = head_w2s.y - math_primitives::s_height_center;
        auto square_root_of = sqrtf(dx * dx + dy * dy);

        if (g_vars->aimbot.display.line) { draw_list->AddLine(ImVec2(math_primitives::s_width_center, math_primitives::s_height_center), ImVec2(head_w2s.x, head_w2s.y), color_list.white_color, 1.5); }

    }
}

#endif