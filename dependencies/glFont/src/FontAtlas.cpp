#include "FontAtlas.h"

#include <algorithm>

FontAtlas::FontAtlas(FT_Face face, int pixelSize) :
  _face(face),
  _width(0),
  _height(0)
{
    _slot = _face->glyph;
    FT_Set_Pixel_Sizes(_face,      // Font face handle
                       0,          // Pixel width  (0 defaults to pixel height)
                       pixelSize); // Pixel height (0 defaults to pixel width)

    int rowWidth = 0;
    int colHeight = 0;

    // Main char set (32 - 128)
    for(int i = 32; i < 128; ++i) {
        if(FT_Load_Char(_face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue; // try next character
        }

        _width += _slot->bitmap.width + 2; // add the width of this glyph to our texture width
        // Note: We add 2 pixels of blank space between glyphs for padding - this helps reduce texture bleeding
        //       that can occur with antialiasing

        _height = std::max(_height, (int)_slot->bitmap.rows);
    }

    // Create texture
    glGenTextures(1, &_tex);
    glActiveTexture(GL_TEXTURE0 + _tex);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Set texture parameters 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create an empty texture with the correct dimensions
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _width, _height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    int texPos = 0; // texture offset

    for(int i = 32; i < 128; ++i) {
        if(FT_Load_Char(_face, i, FT_LOAD_RENDER))
            continue;

        // Add this character glyph to our texture
        glTexSubImage2D(GL_TEXTURE_2D, 0, texPos, 0, 1, _slot->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, (char)0); // padding
        glTexSubImage2D(GL_TEXTURE_2D, 0, texPos, 0, _slot->bitmap.width, _slot->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, _slot->bitmap.buffer);
        glTexSubImage2D(GL_TEXTURE_2D, 0, texPos, 0, 1, _slot->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, (char)0); // padding

        // Store glyph info in our char array for this pixel size
        _chars[i].advanceX = _slot->advance.x >> 6;
        _chars[i].advanceY = _slot->advance.y >> 6;

        _chars[i].bitmapWidth = _slot->bitmap.width;
        _chars[i].bitmapHeight = _slot->bitmap.rows;

        _chars[i].bitmapLeft = _slot->bitmap_left;
        _chars[i].bitmapTop = _slot->bitmap_top;

        _chars[i].xOffset = (float)texPos / (float)_width;

        // Increase texture offset
        texPos += _slot->bitmap.width + 2;
    }
}

FontAtlas::~FontAtlas() {
    glDeleteTextures(1, &_tex);
}
