#pragma once

#include <imgui.h>

namespace FontManager
{
	void Initialize(ImGuiIO& io, int fontSize = 17);
	ImFont* GetUiFont();
	ImFont* GetCodeFont(int desiredSize);

	int GetDefaultUiFontSize();

	int GetMaxCodeFontSize();
	int GetMinCodeFontSize();
}