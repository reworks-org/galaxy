#pragma once

#include <glad/glad.h>
#include "ft2build.h"
#include FT_FREETYPE_H

class GLFont
{
public:
	GLFont(const char* fontFile);
	~GLFont();

	void setFontFile(const char* fontFile);

	FT_Face getFaceHandle();

private:
	char* _fontFile;
	FT_Error _error;
	FT_Library _ft;
	FT_Face _face;
};
