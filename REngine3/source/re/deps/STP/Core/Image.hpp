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

#ifndef STP_IMAGE_HPP
#define STP_IMAGE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <vector>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../Config.hpp"

namespace tmx {

////////////////////////////////////////////////////////////
/// \brief Class for loading images
///
////////////////////////////////////////////////////////////
class STP_API Image {
 public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Image();

    ////////////////////////////////////////////////////////////
    /// \brief Load a image given a source
    ///
    /// \param source The reference to the tileset image file
    /// \param width  The image width in pixels. (Defaults to 0)
    /// \param height The image height in pixels. (Defaults to 0)
    /// \param trans  Defines a specific color that is treated as transparent (example value: 0xFF00FF for magenta)
    /// \param format Used for embedded images, in combination with a data child element
    ///
    ////////////////////////////////////////////////////////////
    Image(const std::string& source, unsigned int width = 0, unsigned int height = 0,
          int32_t trans = -1, const std::string& format = std::string());

 public:
    ////////////////////////////////////////////////////////////
    /// \brief Return the image width
    ///
    /// \return The image width in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the image height
    ///
    /// \return The image height in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return sf::Texture attached to the image
    ///
    /// \return Pointer to a constant sf::Texture
    ///
    ////////////////////////////////////////////////////////////
    const sf::Texture* GetTexture() const;

 private:
    std::string source_;
    int32_t trans_;
    unsigned int width_, height_;
    std::string format_;
    sf::Texture texture_;
};

}  // namespace tmx

#endif  // STP_IMAGE_HPP
