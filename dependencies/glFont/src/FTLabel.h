#pragma once

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>

#include "GLFont.h"

#include <string>
#include <memory> // for use of shared_ptr
#include <map>
#include <vector>

#ifndef PI
#define PI 3.141596
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 180.0 / PI;
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD PI / 180.0
#endif

class FontAtlas;

class FTLabel
{
public:
	enum FontFlags
	{
		LeftAligned      = 1 << 1,
		RightAligned     = 1 << 2,
		CenterAligned    = 1 << 3,
		WordWrap         = 1 << 4,
		Underlined       = 1 << 5,
		Bold             = 1 << 6,
		Italic           = 1 << 7,
		Indented         = 1 << 8,
		HorizontalLayout = 1 << 9
	};

	// Ctor takes a pointer to a font face
	FTLabel(std::shared_ptr<GLFont> ftFace, int windowWidth, int windowHeight);
	FTLabel(GLFont* ftFace, int windowWidth, int windowHeight);
	FTLabel(std::shared_ptr<GLFont> ftFace, const char* text, float x, float y, int width, int height, int windowWidth, int windowHeight);
	FTLabel(std::shared_ptr<GLFont> ftFace, const char* text, float x, float y, int windowWidth, int windowHeight);
	~FTLabel();

	void setWindowSize(int width, int height);

	// Degrees to rotate & axis on which to rotate (e.g. (90 0 1 0) to rotate 90deg on the y axis)
	void rotate(float degrees, float x, float y, float z);
	// NOTE: This does not change the pixel size. Use setPixelSize() to scale evenly
	void scale(float x, float y, float z);

	// Setters
	void setText(char* text);
	void setText(std::string text);
	void setPosition(float x, float y);
	void setSize(int width, int height);
	void setFont(std::shared_ptr<GLFont> ftFace);
	void setColor(float r, float b, float g, float a); // RGBA values are 0 - 1.0
	void setAlignment(FontFlags alignment);
	void setPixelSize(int size);
	void setIndentation(int pixels);
	void setFontFlags(int flags);
	void appendFontFlags(int flags);

	// Getters
	std::string getText();
	float getX();
	float getY();
	int getWidth();
	int getHeight();
	char* getFont();
	glm::vec4 getColor();
	FontFlags getAlignment();
	float getRotation();
	int getIndentation();
	int getFontFlags();

	// Print the specified text on the screen starting at coords x, y
	// NOTE: params x and y should be x and y offsets in **window** coordinates (0, 0 is at the top left corner)
	void drawString(const char* text, float x, float y);
	// Overloaded method: params width and height specify bounding box of the text
	// - Pass 0 as the value for width and/or height to specify no limit
	void drawString(const char* text, float x, float y, float width, float height);

	void render();

private:
	struct Point
	{
		GLfloat x; // x offset in window coordinates
		GLfloat y; // y offset in window coordinates
		GLfloat s; // glyph x offset in texture coordinates
		GLfloat t; // glyph y offset in texture coordinates

		Point()
		{
		}

		Point(float x, float y, float s, float t)
		    : x(x), y(y), s(s), t(t)
		{
		}
	};

	std::shared_ptr<GLFont> _ftFace;
	FT_Face _face;
	FT_Error _error;
	FT_GlyphSlot _g;

	GLuint _programId;
	GLuint _vao;
	GLuint _vbo;

	GLint _uniformTextureHandle;
	GLint _uniformTextColorHandle;
	GLint _uniformMVPHandle;

	std::string _text;

	std::vector<Point> _coords;

	// Holds texture atlases for different character pixel sizes
	std::map<int, std::shared_ptr<FontAtlas>> _fontAtlas;

	int _flags; // Currently enabled settings set via FontFlags
	int _numVertices;

	// Window dimensions
	int _windowWidth;
	int _windowHeight;

	// Coordinates at which to start drawing
	int _x;
	int _y;

	// Label dimensions
	int _width;
	int _height;

	// Used to scale x and y coords
	// Note: sx and sy are chosen so that one glyph pixel corresponds to one screen pixel
	float _sx;
	float _sy;

	// Dimensions of atlas texture
	int _atlasWidth;
	int _atlasHeight;

	glm::mat4 _projection;
	glm::mat4 _view;
	glm::mat4 _model;
	glm::mat4 _mvp;

	char* _font;          // file path to font file
	glm::vec4 _textColor; // RGBA value we will use to color the font (0.0 - 1.0)
	FontFlags _alignment;
	int _pixelSize;
	int _indentationPix;

	bool _isInitialized;

	// Used for debugging opengl only
	inline void getError()
	{
	}

	// Compile shader from file
	void loadShader(char* shaderSource, GLenum shaderType);
	// Calculate offset needed for center- or left-aligned text
	void calculateAlignment(const char* text, float& x);
	// Split text into words separated by spaces
	std::vector<std::string> splitText(const char* text);
	// Returns the width (in pixels) of the string, given the current pixel size
	int calcWidth(const char* text);

	// Calculate vertices for a paragraph label
	void recalculateVertices(const char* text, float x, float y, int width, int height);
	// Calculate vertices without regards to width or height boundaries
	void recalculateVertices(const char* text, float x, float y);

	void recalculateMVP();
};
