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

#ifndef STP_IMAGELAYER_HPP
#define STP_IMAGELAYER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>

#include <SFML/Graphics/Vertex.hpp>

#include "../Config.hpp"
#include "MapObject.hpp"
#include "Image.hpp"

namespace tmx {

////////////////////////////////////////////////////////////
/// \brief Class for manage the TMX ImageLayers
///
////////////////////////////////////////////////////////////
class STP_API ImageLayer : public MapObject {
 public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs an empty image layer with no values.
    ///
    ////////////////////////////////////////////////////////////
    ImageLayer();

    ////////////////////////////////////////////////////////////
    /// \brief Constructs a image layer given a name, width, height
    ///        opacity, visible and image atributes
    ///
    /// \param name    The name of the image layer
    /// \param width   The width of the image layer in tiles
    /// \param height  The height of the image layer in tiles
    /// \param opacity Float value between 0.0 to 1.0
    /// \param visible The visibility of the image layer
    /// \param image   The tmx::Image of the image layer
    ///
    ////////////////////////////////////////////////////////////
    ImageLayer(const std::string& name, unsigned int width,
               unsigned int height, float opacity, bool visible, tmx::Image image);

 public:
    ////////////////////////////////////////////////////////////
    /// \brief Return sf::Texture attached to the image layer
    ///
    /// \return Pointer to a constant sf::Texture
    ///
    ////////////////////////////////////////////////////////////
    const sf::Texture* GetTexture() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the color of the image layer, does not affect the opacity
    ///
    /// \param color sf::Color RGB value
    ///
    ////////////////////////////////////////////////////////////
    void SetColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Change the opacity of the image layer
    ///
    /// \param opacity Float value between 0.0 to 1.0
    ///
    ////////////////////////////////////////////////////////////
    void SetOpacity(float opacity);

 private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 private:
    tmx::Image image_;

    sf::Vertex vertices_[4];
};

}  // namespace tmx

#endif  // STP_IMAGELAYER_HPP
