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

#ifndef STP_OBJECTGROUP_HPP
#define STP_OBJECTGROUP_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <unordered_map>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "../Config.hpp"
#include "MapObject.hpp"
#include "TileSet.hpp"

namespace tmx {

enum ObjectType { Rectangle, Ellipse, Polygon, Polyline, Tile };

////////////////////////////////////////////////////////////
/// \brief Class for manage the TMX ObjectGroups
///
////////////////////////////////////////////////////////////
class STP_API ObjectGroup : public MapObject {
 public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs an empty object group with no values.
    ///
    ////////////////////////////////////////////////////////////
    ObjectGroup();

    ////////////////////////////////////////////////////////////
    /// \brief Constructs a object group given a name, width, height
    ///        opacity, visible and hexcolor atributes
    ///
    /// \param name     The name of the object group
    /// \param width    The width of the object group in tiles
    /// \param height   The height of the object group in tiles
    /// \param opacity  Float value between 0.0 to 1.0
    /// \param visible  The visibility of the object group
    /// \param hexcolor Hexadecimal color used to display the objects in this group. (example value: 0x0000FF for blue)
    ///
    ////////////////////////////////////////////////////////////
    ObjectGroup(const std::string& name, unsigned int width, unsigned int height,
                float opacity, bool visible, int32_t hexcolor = -1);

    ////////////////////////////////////////////////////////////
    /// Nested classes
    ///
    ////////////////////////////////////////////////////////////
    class Object;

    ////////////////////////////////////////////////////////////
    /// \brief Add a new Object to the object group
    ///
    /// \param newobject Object to be added
    ///
    ////////////////////////////////////////////////////////////
    void AddObject(tmx::ObjectGroup::Object newobject);

    ////////////////////////////////////////////////////////////
    /// \brief Change the color of the object group, does not affect the opacity
    ///
    /// \param color sf::Color RGB value
    ///
    ////////////////////////////////////////////////////////////
    void SetColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Change the opacity of the object group
    ///
    /// \param opacity Float value between 0.0 to 1.0
    ///
    ////////////////////////////////////////////////////////////
    void SetOpacity(float opacity);

 private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 private:
    std::vector<tmx::ObjectGroup::Object> objects_;
};

////////////////////////////////////////////////////////////
/// \brief Class for manage each Object inside of the ObjectGroup
///
////////////////////////////////////////////////////////////
class STP_API ObjectGroup::Object : public sf::Drawable, public tmx::Properties {
 public:
    ////////////////////////////////////////////////////////////
    /// \brief Construct a Object given a name, width, height
    ///        rotation, visible and image atributes
    ///
    /// \param name            The name of the object (An arbitrary string)
    /// \param type            The type of the object (An arbitrary string)
    /// \param x               The x coordinate of the object in pixels
    /// \param y               The y coordinate of the object in pixels
    /// \param width           The width of the object in pixels (defaults to 0)
    /// \param height          The width of the object in pixels (defaults to 0)
    /// \param rotation        The rotation of the object in degrees clockwise (defaults to 0)
    /// \param visible         The visibility of the object
    /// \param shape_type      The shape type of the object, see tmx::ObjectType
    /// \param vertices_points String containing a list of coordinates (example: "0,0 17,17 -14,18")
    /// \param tile            Pointer to a Ttmx::TileSet::Tile, only used when is a Tile-Object, otherwise is nullptr.
    ///
    ////////////////////////////////////////////////////////////
    Object(const std::string& name, const std::string& type, int x, int y,
           unsigned int width, unsigned int height, float rotation, bool visible,
           tmx::ObjectType shape_type, const std::string& vertices_points = std::string(),
           tmx::TileSet::Tile* tile = nullptr);

 public:
    ////////////////////////////////////////////////////////////
    /// \brief Change the color of the tile, affect the opacity.
    ///
    /// \param color sf::Color RGBA value
    ///
    ////////////////////////////////////////////////////////////
    void SetColor(const sf::Color& color);

 private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 private:
    std::string name_;
    std::string type_;
    unsigned int x_, y_;
    unsigned int width_, height_;
    float rotation_;

    tmx::TileSet::Tile* tile_;
    std::vector<sf::Vertex> vertices_;

 public:
    /// \brief Visibility of the Object
    bool visible;
};

}  // namespace tmx

#endif  // STP_OBJECTGROUP_HPP
