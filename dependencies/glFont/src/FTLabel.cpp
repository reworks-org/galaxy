#include "FTLabel.h"
#include "GLUtils.h"
#include "FontAtlas.h"

#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

// GLM
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\transform.hpp"

FTLabel::FTLabel(shared_ptr<GLFont> ftFace, int windowWidth, int windowHeight) :
  _isInitialized(false),
  _text(""),
  _alignment(FontFlags::LeftAligned),
  _textColor(0, 0, 0, 1),
  _uniformMVPHandle(-1),
  _indentationPix(0),
  _x(0),
  _y(0),
  _width(0),
  _height(0)
{
    setFont(ftFace);
    setWindowSize(windowWidth, windowHeight);

    // Intially enabled flags
    _flags = FontFlags::LeftAligned | FontFlags::WordWrap;

    // Since we are dealing with 2D text, we will use an orthographic projection matrix 
    _projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, // View: left, right, bottom, top (we are using normalized coords)
                             0.1f,                     // Near clipping distance
                             100.0f);                  // Far clipping distance

    _view = glm::lookAt(glm::vec3(0, 0, 1),  // Camera position in world space (doesn't really apply when using an ortho projection matrix)
                        glm::vec3(0, 0, 0),  // look at origin
                        glm::vec3(0, 1, 0)); // Head is up (set to 0, -1, 0 to look upside down)

    _model = glm::mat4(1.0f); // Identity matrix

    recalculateMVP();

    // Load the shaders
    _programId = glCreateProgram();
    GLUtils::loadShader("shaders\\fontVertex.shader", GL_VERTEX_SHADER, _programId);
    GLUtils::loadShader("shaders\\fontFragment.shader", GL_FRAGMENT_SHADER, _programId);

    glUseProgram(_programId);

    // Create and bind the vertex array object
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Set default pixel size and create the texture
    setPixelSize(48); // default pixel size

    // Get shader handles
    _uniformTextureHandle = glGetUniformLocation(_programId, "tex");
    _uniformTextColorHandle = glGetUniformLocation(_programId, "textColor");
    _uniformMVPHandle = glGetUniformLocation(_programId, "mvp");

    GLuint curTex = _fontAtlas[_pixelSize]->getTexId(); // get texture ID for this pixel size

    glActiveTexture(GL_TEXTURE0 + curTex);
    glBindTexture(GL_TEXTURE_2D, curTex);

    // Set our uniforms
    glUniform1i(_uniformTextureHandle, curTex);
    glUniform4fv(_uniformTextColorHandle, 1, glm::value_ptr(_textColor));
    glUniformMatrix4fv(_uniformMVPHandle, 1, GL_FALSE, glm::value_ptr(_mvp));

    // Create the vertex buffer object
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glUseProgram(0);

    _isInitialized = true;
}

FTLabel::FTLabel(GLFont* ftFace, int windowWidth, int windowHeight) : 
  FTLabel(std::shared_ptr<GLFont>(new GLFont(*ftFace)), windowWidth, windowHeight) 
{}

FTLabel::FTLabel(shared_ptr<GLFont> ftFace, const char* text, float x, float y, int width, int height, int windowWidth, int windowHeight) :
  FTLabel(ftFace, text, x, y, windowWidth, windowHeight)
{
    _width = width;
    _height = height;

    recalculateVertices(text, x, y, width, height);
}

FTLabel::FTLabel(shared_ptr<GLFont> ftFace, const char* text, float x, float y, int windowWidth, int windowHeight) :
FTLabel(ftFace, windowWidth, windowHeight)
{
    _text = (char*)text;
    _x = x;
    _y = y;
}

FTLabel::~FTLabel() {
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}

void FTLabel::recalculateVertices(const char* text, float x, float y, int width, int height) {
    
    _coords.clear(); // case there are any existing coords

    // Break the text into individual words
    vector<string> words = splitText(_text);

    vector<string> lines;
    int widthRemaining = width;
    int spaceWidth = calcWidth(" ");
    int indent = (_flags & FontFlags::Indented) && _alignment != FontFlags::CenterAligned ? _pixelSize : 0;

    // Create lines from our text, each containing the maximum amount of words we can fit within the given width
    string curLine = "";
    for(string word : words) {
        int wordWidth = calcWidth(word.c_str());

        if(wordWidth - spaceWidth > widthRemaining && width /* make sure there is a width specified */) {

            // If we have passed the given width, add this line to our collection and start a new line
            lines.push_back(curLine);
            widthRemaining = width - wordWidth;
            curLine = "";

            // Start next line with current word
            curLine.append(word.c_str());
        }
        else {
            // Otherwise, add this word to the current line
            curLine.append(word.c_str());
            widthRemaining = widthRemaining - wordWidth;
        }
    }

    // Add the last line to lines
    if(curLine != "")
        lines.push_back(curLine);

    // Print each line, increasing the y value as we go
    float startY = y - (_face->size->metrics.height >> 6);
    for(string line : lines) {
        // If we go past the specified height, stop drawing
        if(y - startY > height && height)
            break;

        recalculateVertices(line.c_str(), x + indent, y);
        y += (_face->size->metrics.height >> 6);
        indent = 0;
    }
    glUseProgram(_programId);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint curTex = _fontAtlas[_pixelSize]->getTexId();
    glActiveTexture(GL_TEXTURE0 + curTex);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBindTexture(GL_TEXTURE_2D, curTex);
    glUniform1i(_uniformTextureHandle, curTex);

    glEnableVertexAttribArray(0);

    // Send the data to the gpu
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Point), 0);
    glBufferData(GL_ARRAY_BUFFER, _coords.size() * sizeof(Point), _coords.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, _coords.size());
    _numVertices = _coords.size();

    glDisableVertexAttribArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisable(GL_BLEND);
    glUseProgram(0);
}

void FTLabel::recalculateVertices(const char* text, float x, float y) {

    // Coordinates passed in should specify where to start drawing from the top left of the text,
    // but FreeType starts drawing from the bottom-right, therefore move down one line
    y += _face->size->metrics.height >> 6;

    FT_GlyphSlot slot = _ftFace->getFaceHandle()->glyph;

    // Calculate alignment (if applicable)
    int textWidth = calcWidth(text);
    if(_alignment == FontFlags::CenterAligned)
        x -= textWidth / 2.0;
    else if(_alignment == FontFlags::RightAligned)
        x -= textWidth;

    // Normalize window coordinates
    x = -1 + x * _sx;
    y = 1 - y * _sy;

    
    int atlasWidth = _fontAtlas[_pixelSize]->getAtlasWidth();
    int atlasHeight = _fontAtlas[_pixelSize]->getAtlasHeight();

    FontAtlas::Character* chars = _fontAtlas[_pixelSize]->getCharInfo();

    for(const char *p = text; *p; ++p) {
        float x2 = x + chars[*p].bitmapLeft * _sx; // scaled x coord
        float y2 = -y - chars[*p].bitmapTop * _sy; // scaled y coord
        float w = chars[*p].bitmapWidth * _sx;     // scaled width of character
        float h = chars[*p].bitmapHeight * _sy;    // scaled height of character

        // Calculate kerning value
        FT_Vector kerning;
        FT_Get_Kerning(_face,              // font face handle
                       *p,                 // left glyph
                       *(p + 1),           // right glyph
                       FT_KERNING_DEFAULT, // kerning mode
                       &kerning);          // variable to store kerning value

        // Advance cursor to start of next character
        x += (chars[*p].advanceX + (kerning.x >> 6)) * _sx;
        y += chars[*p].advanceY * _sy;

        // Skip glyphs with no pixels (e.g. spaces)
        if(!w || !h)
            continue;

        _coords.push_back(Point(x2,                // window x
                                -y2,               // window y
                                chars[*p].xOffset, // texture atlas x offset
                                0));               // texture atlas y offset

        _coords.push_back(Point(x2 + w,
                                -y2,
                                chars[*p].xOffset + chars[*p].bitmapWidth / atlasWidth,
                                0));

        _coords.push_back(Point(x2,
                                -y2 - h,
                                chars[*p].xOffset,
                                chars[*p].bitmapHeight / atlasHeight));

        _coords.push_back(Point(x2 + w,
                                -y2,
                                chars[*p].xOffset + chars[*p].bitmapWidth / atlasWidth,
                                0));

        _coords.push_back(Point(x2,
                                -y2 - h,
                                chars[*p].xOffset,
                                chars[*p].bitmapHeight / atlasHeight));

        _coords.push_back(Point(x2 + w,
                                -y2 - h,
                                chars[*p].xOffset + chars[*p].bitmapWidth / atlasWidth,
                                chars[*p].bitmapHeight / atlasHeight));
    }

}

void FTLabel::render() {
    glUseProgram(_programId);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint curTex = _fontAtlas[_pixelSize]->getTexId();
    glActiveTexture(GL_TEXTURE0 + curTex);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBindTexture(GL_TEXTURE_2D, curTex);
    glUniform1i(_uniformTextureHandle, curTex);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Point), 0);
    glDrawArrays(GL_TRIANGLES, 0, _numVertices);

    glDisableVertexAttribArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisable(GL_BLEND);
    glUseProgram(0);
}

vector<string> FTLabel::splitText(const char* text) {
    string textStr = string(text);
    vector<string> words;
    int startPos = 0; // start position of current word
    int endPos = textStr.find(' '); // end position of current word

    if(endPos == -1) {
        // There is only one word, so return early
        words.push_back(text);
        return words;
    }

    // Find each word in the text (delimited by spaces) and add it to our vector of words
    while(endPos != string::npos) {
        words.push_back(textStr.substr(startPos, endPos  - startPos + 1));
        startPos = endPos + 1;
        endPos = textStr.find(' ', startPos);
    }

    // Add last word
    words.push_back(textStr.substr(startPos, std::min(endPos, (int)textStr.size()) - startPos + 1));

    return words;
}

int FTLabel::calcWidth(const char* text) {
    int width = 0;
    FontAtlas::Character* chars = _fontAtlas[_pixelSize]->getCharInfo();
    for(const char* p = text; *p; ++p) {
        width += chars[*p].advanceX;
    }

    return width;
}

void FTLabel::setText(char* text) {
    _text = text;

    recalculateVertices(_text, _x, _y, _width, _height);
}

void FTLabel::setText(string text) {
    setText(text.c_str());
}

string FTLabel::getText() {
    return string(_text);
}

void FTLabel::setPosition(float x, float y) {
    _x = x;
    _y = y;

    if(_text != "") {
        recalculateVertices(_text, _x, _y, _width, _height);
    }
}

float FTLabel::getX() {
    return _x;
}

float FTLabel::getY() {
    return _y;
}

void FTLabel::setSize(int width, int height) {
    _width = width;
    _height = height;

    if(_text != "") {
        recalculateVertices(_text, _x, _y, width, height);
    }
}

int FTLabel::getWidth() {
    return _width;
}

int FTLabel::getHeight() {
    return _height;
}

void FTLabel::setFontFlags(int flags) {
    _flags = flags;
}

void FTLabel::appendFontFlags(int flags) {
    _flags |= flags;
}

int FTLabel::getFontFlags() {
    return _flags;
}

void FTLabel::setIndentation(int pixels) {
    _indentationPix = pixels;
}

int FTLabel::getIndentation() {
    return _indentationPix;
}

void FTLabel::setColor(float r, float b, float g, float a) {
    _textColor = glm::vec4(r, b, g, a);

    // Update the textColor uniform
    if(_programId != -1) {
        glUseProgram(_programId);
        glUniform4fv(_uniformTextColorHandle, 1, glm::value_ptr(_textColor));
        glUseProgram(0);
    }
}

glm::vec4 FTLabel::getColor() {
    return _textColor;
}

void FTLabel::setFont(shared_ptr<GLFont> ftFace) {
    _ftFace = ftFace;
    _face = _ftFace->getFaceHandle(); // shortcut
}

char* FTLabel::getFont() {
    return _font;
}

void FTLabel::setAlignment(FTLabel::FontFlags alignment) {
    _alignment = alignment;

    if(_isInitialized) {
        recalculateVertices(_text, _x, _y, _width, _height);
    }
}

FTLabel::FontFlags FTLabel::getAlignment() {
    return _alignment;
}

void FTLabel::calculateAlignment(const char* text, float &x) {
    if(_alignment == FTLabel::FontFlags::LeftAligned)
        return; // no need to calculate alignment

    FT_GlyphSlot slot = _face->glyph;
    int totalWidth = 0; // total width of the text to render in window space
    FontAtlas::Character* chars = _fontAtlas[_pixelSize]->getCharInfo();
   
    // Calculate total width
    for(const char* p = text; *p; ++p)
        totalWidth += chars[*p].advanceX;

    if(_alignment == FTLabel::FontFlags::CenterAligned)
        x -= totalWidth / 2.0;
    else if(_alignment == FTLabel::FontFlags::RightAligned)
        x -= totalWidth;
}

void FTLabel::setPixelSize(int size) {
    _pixelSize = size;

    // Create texture atlas for characters of this pixel size if there isn't already one
    if(!_fontAtlas[_pixelSize])
        _fontAtlas[_pixelSize] = std::shared_ptr<FontAtlas>(new FontAtlas(_face, _pixelSize));

    if(_isInitialized) {
        recalculateVertices(_text, _x, _y, _width, _height);
    }
}

void FTLabel::setWindowSize(int width, int height) {
    _windowWidth = width;
    _windowHeight = height;

    // Recalculate sx and sy
    _sx = 2.0 / _windowWidth;
    _sy = 2.0 / _windowHeight;
    
    if(_isInitialized) {
        recalculateVertices(_text, _x, _y, _width, _height);
    }
}

void FTLabel::rotate(float degrees, float x, float y, float z) {
    float rad = degrees * DEG_TO_RAD;
    _model = glm::rotate(_model, rad, glm::vec3(x, y, z));
    recalculateMVP();
}

void FTLabel::scale(float x, float y, float z) {
    _model = glm::scale(glm::vec3(x, y, z));
    recalculateMVP();
}

void FTLabel::recalculateMVP() {
    _mvp = _projection * _view * _model;

    if(_uniformMVPHandle != -1) {
        glUseProgram(_programId);
        glUniformMatrix4fv(_uniformMVPHandle, 1, GL_FALSE, glm::value_ptr(_mvp));
        glUseProgram(0);
    }
}