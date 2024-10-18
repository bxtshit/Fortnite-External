#pragma once
#include "../../dependencies/imgui/imgui.h"

class drawing_
{
public:
	void text(ImDrawList* drawlist, int x, int y, ImColor Color, const char* text, bool outlined)
	{
		if (outlined)
		{
			ImVec2 offsets[] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

			for (const auto& offset : offsets)
			{
				drawlist->AddText(ImVec2(x + offset.x, y + offset.y), ImColor(0, 0, 0, 255), text);
			}
		}

		drawlist->AddText(ImVec2(x, y), Color, text);
	}

	void box_2d(ImDrawList* drawlist, float X, float Y, float W, float H, ImColor col, int thickness)
	{
		drawlist->AddRect(ImVec2(X - 1, Y - 1), ImVec2(X + W + 1, Y + H + 1), ImColor(0, 0, 0, 255), 0, 0, thickness);
	}

	void box_cornered(ImDrawList* drawlist, int X, int Y, int W, int H, ImColor color, int thickness)
	{
		float lineW = static_cast<float>(W) / 3.0f;
		float lineH = static_cast<float>(H) / 3.0f;
		float scaled_thick = static_cast<float>(thickness);

		drawlist->AddLine(ImVec2(X - scaled_thick, Y - scaled_thick), ImVec2(X + lineW + scaled_thick, Y - scaled_thick), IM_COL32_BLACK, scaled_thick);
		drawlist->AddLine(ImVec2(X - scaled_thick, Y - scaled_thick), ImVec2(X - scaled_thick, Y + lineH + scaled_thick), IM_COL32_BLACK, scaled_thick);
		drawlist->AddLine(ImVec2(X + W - lineW - scaled_thick, Y - scaled_thick), ImVec2(X + W + scaled_thick, Y - scaled_thick), IM_COL32_BLACK, scaled_thick);
		drawlist->AddLine(ImVec2(X + W + scaled_thick, Y - scaled_thick), ImVec2(X + W + scaled_thick, Y + lineH + scaled_thick), IM_COL32_BLACK, scaled_thick);

		drawlist->AddLine(ImVec2(X - scaled_thick, Y + H - lineH - scaled_thick), ImVec2(X - scaled_thick, Y + H + scaled_thick), IM_COL32_BLACK, scaled_thick);
		drawlist->AddLine(ImVec2(X - scaled_thick, Y + H + scaled_thick), ImVec2(X + lineW + scaled_thick, Y + H + scaled_thick), IM_COL32_BLACK, scaled_thick);
		drawlist->AddLine(ImVec2(X + W - lineW - scaled_thick, Y + H - scaled_thick), ImVec2(X + W + scaled_thick, Y + H - scaled_thick), IM_COL32_BLACK, scaled_thick);
		drawlist->AddLine(ImVec2(X + W + scaled_thick, Y + H - lineH - scaled_thick), ImVec2(X + W + scaled_thick, Y + H + scaled_thick), IM_COL32_BLACK, scaled_thick);

	}
};
inline drawing_ drawing;