#pragma once
#include "../../../dependencies/imgui/imgui.h"
#include <d3d11.h>

inline int tab = 0;

class menu
{
public:
    void draw();
    void set_style();
    menu() : is_opening(true), is_closing(false), animation_progress(0.0f) {}

    void toggle()
    {
        if (!is_opening && !is_closing)
        {
            if (show_menu)
            {
                is_closing = true;
            }
            else
            {
                is_opening = true;
            }
        }
    }

private:
    bool is_opening;
    bool is_closing;
    float animation_progress;
    bool show_menu = false;
};

inline menu menu_ptr;

namespace font
{
    inline ImFont* title;
    inline ImFont* medium;
    inline ImFont* large;
    inline ImFont* regular;
}