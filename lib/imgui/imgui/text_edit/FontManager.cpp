#include "FontManager.h"

#include <string>
#include <unordered_map>

#include "FiraCode/FiraCode-Regular.h"
#include "Inter/Inter.h"

#define MIN_CODE_FONT_SIZE 7
#define MAX_CODE_FONT_SIZE 40

namespace FontManager {
	int defaultUiFontSize;

	ImFont* uiFont;
	std::unordered_map<int, ImFont*> codeFonts;
}

void FontManager::Initialize(ImGuiIO& io, int fontSize)
{
	defaultUiFontSize = fontSize;

	ImFontConfig font_cfg         = {};
	font_cfg.FontDataOwnedByAtlas = false;

	uiFont = io.Fonts->AddFontFromMemoryTTF((void*)Inter_ttc, Inter_ttc_len, fontSize, &font_cfg);
	
	for (int i = MIN_CODE_FONT_SIZE; i <= MAX_CODE_FONT_SIZE; i++)
				codeFonts[i] = io.Fonts->AddFontFromMemoryTTF((void*)FiraCode_Regular_ttf, FiraCode_Regular_ttf_len, i, &font_cfg);
}


ImFont* FontManager::GetUiFont()
{
	return uiFont;
}

ImFont* FontManager::GetCodeFont(int desiredSize)
{
	if (desiredSize > MAX_CODE_FONT_SIZE)
	{
		desiredSize = MAX_CODE_FONT_SIZE;
	}
	else if (desiredSize < MIN_CODE_FONT_SIZE)
	{
		desiredSize = MIN_CODE_FONT_SIZE;
	}
	return codeFonts[desiredSize];
}

int FontManager::GetDefaultUiFontSize()
{
	return defaultUiFontSize;
}

int FontManager::GetMaxCodeFontSize()
{
	return MAX_CODE_FONT_SIZE;
}

int FontManager::GetMinCodeFontSize()
{
	return MIN_CODE_FONT_SIZE;
}
