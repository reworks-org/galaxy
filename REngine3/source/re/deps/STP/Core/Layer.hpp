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

#ifndef STP_LAYER_HPP
#define STP_LAYER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <vector>

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "../Config.hpp"
#include "TileSet.hpp"
#include "MapObject.hpp"

namespace tmx {

////////////////////////////////////////////////////////////
/// \brief Class for manage the TMX Layers
///
////////////////////////////////////////////////////////////
class STP_API Layer : public MapObject {
 public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs an empty layer with no values.
    ///
    ////////////////////////////////////////////////////////////
    Layer();

    ////////////////////////////////////////////////////////////
    /// \brief Constructs a layer given a name, width, height,
    ///        opacity and visible attributes
    ///
    /// \param name    The name of the layer
    /// \param width   The width of the layer in tiles
    /// \param height  The height of the layer in tiles
    /// \param opacity Float value between 0.0 to 1.0
    /// \param visible The visibility of the layer
    ///
    ////////////////////////////////////////////////////////////
    Layer(const std::string& name, unsigned int width,
          unsigned int height, float opacity, bool visible, std::string orientation);

    ////////////////////////////////////////////////////////////
    /// Nested classes
    ///
    ////////////////////////////////////////////////////////////
    class Tile;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the width in tiles.
    ///
    /// \return The width of the layer in tiles.
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the height in tiles.
    ///
    /// \return The height of the layer in tiles.
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the tile given a coordinate. Left-Up tile is (0, 0).
    ///
    /// \param x The x position of the Tile
    /// \param y The y position of the Tile
    ///
    /// \exception std::out_of_range If no tile within the range of the layer.
    ///
    /// \return Reference to the Tile.
    ///
    ////////////////////////////////////////////////////////////
    tmx::Layer::Tile& GetTile(unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// \brief Change the color of the layer, does not affect the opacity
    ///
    /// \param color sf::Color RGB value
    ///
    ////////////////////////////////////////////////////////////
    void SetColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Change the opacity of the layer
    ///
    /// \param opacity Float value between 0.0 to 1.0
    ///
    ////////////////////////////////////////////////////////////
    void SetOpacity(float opacity);

 private:
    ////////////////////////////////////////////////////////////
    /// \brief Friend class
    ///
    ////////////////////////////////////////////////////////////
    friend class Parser;

	void AddTile(unsigned int gid, sf::IntRect tile_rect, tmx::TileSet* tileset = nullptr);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<tmx::Layer::Tile> tiles_;
    std::string orientation_;
};

////////////////////////////////////////////////////////////
/// \brief Class for manage each Tile inside a Layer
///
////////////////////////////////////////////////////////////
class STP_API Layer::Tile : public sf::Drawable {
 public:
	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Constructs an empty tile with no values.
	///
	////////////////////////////////////////////////////////////
	Tile();

	////////////////////////////////////////////////////////////
	/// \brief Constructor that receives the gid, tile_rect and a pointer to the tileset
	///
    /// \param gid       The global id of the tmx::TileSet::Tile attached.
    /// \param tile_rect The global bounds of the tile.
    /// \param tileset   A pointer to a tmx::TileSet to get the texture.
	///
	////////////////////////////////////////////////////////////
	Tile(unsigned int gid, sf::IntRect tile_rect, std::string orientation,
	     tmx::TileSet* tileset = nullptr);

    ////////////////////////////////////////////////////////////
    /// \brief Check if the tile is empty (Doesn't have a texture).
    ///
    /// \return true if the tile is empty, false otherwise.
    ///
    ////////////////////////////////////////////////////////////
    bool empty() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the global bounds of the tile
    ///
    /// \return Global bounds of the tile
    ///
    ////////////////////////////////////////////////////////////
    sf::FloatRect GetGlobalBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the color of the tile, affect the opacity.
    ///
    /// \param color sf::Color RGBA value
    ///
    ////////////////////////////////////////////////////////////
    void SetColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Add new property.
    ///
    /// \param name  The name of the property
    /// \param value The value of the property
    ///
    ////////////////////////////////////////////////////////////
    void AddProperty(const std::string& name, const std::string& value);

    ////////////////////////////////////////////////////////////
    /// \brief Return a property value given a name
    ///
    /// \param name The name of the property
    ///
    /// \return Reference to the property value
    ///
    ////////////////////////////////////////////////////////////
    std::string& GetPropertyValue(const std::string& name);

 private:
    friend class tmx::Layer;

 private:
    unsigned int gid_;

    sf::Vertex vertices_[4];
    sf::IntRect tile_rect_;

    tmx::Properties* tile_properties_;

    const sf::Texture* texture_;
    sf::IntRect texture_rect_;

    void setTexture(unsigned int gid);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void UpdatePositions();
    void UpdateTexCoords();

 public:
    /// \brief Visibility of the Tile
    bool visible;
};

}  // namespace tmx

#endif  // STP_LAYER_HPP
