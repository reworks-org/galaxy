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

#ifndef STP_TILESET_HPP
#define STP_TILESET_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "../Config.hpp"
#include "Properties.hpp"
#include "Image.hpp"

namespace tmx {

////////////////////////////////////////////////////////////
/// \brief Class for manage the TMX TileSets
///
////////////////////////////////////////////////////////////
class STP_API TileSet : public tmx::Properties {
 public:
    class Tile;

    struct TerrainTypes {  // 0.2
    };

 public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    TileSet();

    ////////////////////////////////////////////////////////////
    /// \brief Constructs a tileset
    ///
    /// \param firstgid   The first global tile ID of this tileset (this global ID maps to the first tile in this tileset)
    /// \param name       The name of the tileset in tiles
    /// \param tilewidth  The width of the tiles in pixels
    /// \param tileheight The height of the tiles in pixels
    /// \param image      The tmx::Image attached to it
    /// \param spacing    The spacing in pixels between the tiles
    /// \param margin     The margin around the tiles
    /// \param tileoffset Offset in pixels, to be applied when drawing a tile from the related tileset
    ///
    ////////////////////////////////////////////////////////////
    TileSet(unsigned int firstgid, const std::string& name, unsigned int tilewidth,
            unsigned int tileheight, tmx::Image image, unsigned int spacing = 0,
            unsigned int margin = 0, sf::Vector2i tileoffset = {0, 0});

    ////////////////////////////////////////////////////////////
    /// \brief Get the tile given a local id.
    ///
    /// \param id The local id of the tile
    /// 
    /// \exception std::out_of_range If the local id is not within the range of the tileset.
    ///
    /// \return Reference to the Tile.
    ///
    ////////////////////////////////////////////////////////////
    tmx::TileSet::Tile& GetTile(unsigned int id);

    //////////////////////////////////////////////////////////// 
    /// \brief Returns a sf::IntRect with the position of the
    ///        tile texture in the attached image
    ///
    /// \param id The local id of the tile
    ///
    /// \exception std::out_of_range If the local id is not within the range of the tileset.
    ///
    /// \return The position of the tile texture in the image
    ///
    ////////////////////////////////////////////////////////////
    sf::IntRect GetTextureRect(unsigned int id) const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the texture attached to the tileset
    ///
    /// \return Pointer to a constant sf::Texture
    ///
    ////////////////////////////////////////////////////////////
    const sf::Texture* GetTexture() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the tile offset
    ///
    /// \return sf::Vector2i with the offset
    ///
    ////////////////////////////////////////////////////////////
    const sf::Vector2i GetTileOffSet() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the tileset name
    ///
    /// \return The name of the tileset
    ///
    ////////////////////////////////////////////////////////////
    const std::string& GetName() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the width of the tiles in this tileset.
    ///
    /// \return The width of the tiles in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetTileWidth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the height of the tiles in this tileset.
    ///
    /// \return The height of the tiles in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetTileHeight() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the first global tile ID of this tileset
    ///
    /// \return Global tile ID of the first tile in this tileset
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetFirstGID() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the last global tile ID of this tileset
    ///
    /// \return Global tile ID of the last tile in this tileset
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetLastGID() const;

 private:
    unsigned int firstgid_, lastgid_;
    std::string name_;
    unsigned int tilewidth_, tileheight_, spacing_, margin_;
    unsigned int width_no_spacing_, height_no_spacing_;
    tmx::Image image_;
    sf::Vector2i tileoffset_;

    std::vector<tmx::TileSet::Tile> tiles_;
};


////////////////////////////////////////////////////////////
/// \brief Class for manage each Tile inside the TileSet
///
////////////////////////////////////////////////////////////
class STP_API TileSet::Tile : public tmx::Properties {
 public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Tile();

    ////////////////////////////////////////////////////////////
    /// \brief Returns the texture where is the Tile.
    ///
    /// \return Pointer to a constant sf::Texture
    ///
    ////////////////////////////////////////////////////////////
    const sf::Texture* GetTexture() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns a sf::IntRect with the position of the
    ///        tile texture in the attached image
    ///
    /// \return The position of the tile texture in the image
    ///
    ////////////////////////////////////////////////////////////
    sf::IntRect GetTextureRect() const;

 private:
    Tile(unsigned int id, sf::IntRect texture_rect, const tmx::TileSet* parent);

    friend class tmx::TileSet;

 private:
    unsigned int id_;
    const tmx::TileSet* parent_;
    sf::IntRect texture_rect_;
};

}  // namespace tmx

#endif  // STP_TILESET_HPP
