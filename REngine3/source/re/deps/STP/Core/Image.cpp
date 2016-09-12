////////////////////////////////////////////////////////////
//
// The MIT License (MIT)
//
// STP - SFML TMX Parser
// Copyright (c) 2013-2014 Manuel Sabogal
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////

#include "Image.hpp"

#include <string>

namespace tmx {

Image::Image() {}

Image::Image(const std::string& source, unsigned int width,
             unsigned int height, int32_t trans, const std::string& format) :
        source_(source),
        trans_(trans),
        width_(width),
        height_(height),
        format_(format) {
    sf::Image newimage;
    newimage.loadFromFile(source);
    if (trans >= 0) {
        unsigned char red, green, blue;
        red = trans >> 16;
        green = (trans >> 8) & 0xff;
        blue = trans & 0xff;
        newimage.createMaskFromColor(sf::Color(red, green, blue));
    }
    texture_.loadFromImage(newimage);
    if (width == 0 && height == 0) {
        width_ = texture_.getSize().x;
        height_ = texture_.getSize().y;
    }
}

unsigned int Image::GetWidth() const {
    return width_;
}

unsigned int Image::GetHeight() const {
    return height_;
}

const sf::Texture* Image::GetTexture() const {
    return &texture_;
}

}  // namespace tmx
