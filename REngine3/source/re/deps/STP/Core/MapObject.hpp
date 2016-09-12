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

#ifndef STP_MAPOBJECT_HPP
#define STP_MAPOBJECT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>

#include "../Config.hpp"
#include "Properties.hpp"

namespace tmx {

////////////////////////////////////////////////////////////
/// \brief Abstract base class for managing the Tiled elements:
///        Layer, ObjectGroup, ImageLayer
///
////////////////////////////////////////////////////////////
class STP_API MapObject : public sf::Drawable, public tmx::Properties {
 public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    MapObject();

    ////////////////////////////////////////////////////////////
    /// \brief Constructor with common attributes of the map object
    ///
    /// \param name    Name of the Tiled element
    /// \param width   Width of the Tiled element
    /// \param height  Height of the Tiled element
    /// \param opacity Opacity of the Tiled element
    /// \param visible Visibility of the Tiled element
    ///
    ////////////////////////////////////////////////////////////
    MapObject(const std::string& name, unsigned int width,
              unsigned int height, float opacity, bool visible);

    ////////////////////////////////////////////////////////////
    /// \brief Default virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~MapObject();

 public:
    ////////////////////////////////////////////////////////////
    /// \brief Return the name of the Tiled element
    ///
    /// \return Reference to a const string
    ///
    ////////////////////////////////////////////////////////////
    const std::string& GetName() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the color of the Tiled element, does not affect the opacity
    ///
    /// \param color sf::Color RGB value
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetColor(const sf::Color& color) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the opacity of the Tiled element
    ///
    /// \param opacity Float value between 0.0 to 1.0
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetOpacity(float opacity) = 0;

 private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

 protected:
    /// \brief Name of the Tiled element
    std::string name_;
    /// \brief Width of the Tiled element
    unsigned int width_;
    /// \brief Height of the Tiled element
    unsigned int height_;
    /// \brief Opacity of the Tiled element
    float opacity_;
    /// \brief Color of the Tiled element
    sf::Color color_;

 public:
    /// \brief Visibility of the Tiled element
    bool visible;
};

}  // namespace tmx

#endif  // STP_MAPOBJECT_HPP
