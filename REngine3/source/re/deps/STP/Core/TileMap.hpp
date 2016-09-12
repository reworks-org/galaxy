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

#ifndef STP_TILEMAP_HPP
#define STP_TILEMAP_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "../Config.hpp"
#include "Properties.hpp"
#include "TileSet.hpp"
#include "Layer.hpp"
#include "ObjectGroup.hpp"
#include "ImageLayer.hpp"

namespace tmx {

////////////////////////////////////////////////////////////
/// \brief Main class to manage the TMX Map Format
///
////////////////////////////////////////////////////////////
class STP_API TileMap : public sf::Drawable, public tmx::Properties {
 public:
    ////////////////////////////////////////////////////////////
    /// \brief Parse and load the TMX file given a path to it
    ///
    /// \param tmx_file Path to the TMX file
    ///
    ////////////////////////////////////////////////////////////
    explicit TileMap(const std::string& tmx_file);

 private:
    TileMap(const TileMap& other) = delete;
    TileMap& operator =(const TileMap&) = delete;

 public:
    ////////////////////////////////////////////////////////////
    /// \brief Return the tile set attached to the global id
    ///
    /// \param gid The gid of the tile
    ///
    /// \return Pointer to a tmx::TileSet or nullptr if 
    ///         the gid does not exist or is 0
    ///
    ////////////////////////////////////////////////////////////
    tmx::TileSet* GetTileSet(unsigned int gid);

    ////////////////////////////////////////////////////////////
    /// \brief Return the tile set given a name
    ///
    /// \param tileset_name The name of the tile set
    ///
    /// \return Pointer to a constant tmx::TileSet
    ///
    ////////////////////////////////////////////////////////////
    tmx::TileSet& GetTileSet(const std::string& tileset_name);

    ////////////////////////////////////////////////////////////
    /// \brief Return the layer based on its name
    ///
    /// \param layer_name The name of the layer
    ///
    /// \return Reference to a tmx::Layer
    ///
    ////////////////////////////////////////////////////////////
    tmx::Layer& GetLayer(const std::string& layer_name);

    ////////////////////////////////////////////////////////////
    /// \brief Return the object group based on its name
    ///
    /// \param objectgroup_name The name of the object group
    ///
    /// \return Reference to a tmx::ObjectGroup
    ///
    ////////////////////////////////////////////////////////////
    tmx::ObjectGroup& GetObjectGroup(const std::string& objectgroup_name);

    ////////////////////////////////////////////////////////////
    /// \brief Return the image layer based on its name
    ///
    /// \param imagelayer_name The name of the image layer
    ///
    /// \return Reference to a tmx::ImageLayer
    ///
    ////////////////////////////////////////////////////////////
    tmx::ImageLayer& GetImageLayer(const std::string& imagelayer_name);

    ////////////////////////////////////////////////////////////
    /// \brief Display the objects in the map
    ///
    /// \param show true, displays it\n
    ///             false, hides it
    ///
    /// \return Reference to a tmx::ImageLayer
    ///
    ////////////////////////////////////////////////////////////
    void ShowObjects(bool show = true);

    ////////////////////////////////////////////////////////////
    /// \brief Return the map width
    ///
    /// \return unsigned int value
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the map height
    ///
    /// \return unsigned int value
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the tile width
    ///
    /// \return unsigned int value
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetTileWidth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the tile height
    ///
    /// \return unsigned int value
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetTileHeight() const;
    
    ////////////////////////////////////////////////////////////
    /// \brief Return the orientation of the map
    ///
    /// \return The orienation of the map as std::string
    ///
    ////////////////////////////////////////////////////////////
    const std::string& GetOrientation() const;

 private:
    void AddLayer(tmx::Layer* newlayer);
    void AddObjectGroup(tmx::ObjectGroup* newobjectgroup);
    void AddImageLayer(tmx::ImageLayer* newimagelayer);
    void AddTileSet(tmx::TileSet* newtileset);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 private:
    std::string working_dir_;

    float version_;
    std::string orientation_;
    unsigned int width_, height_, tilewidth_, tileheight_;

    std::unordered_map<std::string, tmx::Layer*> layers_;
    std::unordered_map<std::string, tmx::ObjectGroup*> object_groups_;
    std::unordered_map<std::string, tmx::ImageLayer*> image_layers_;
    std::vector<std::unique_ptr<tmx::MapObject>> map_objects_;

    std::unordered_map<std::string, tmx::TileSet*> tilesets_hash_;
    std::vector<std::unique_ptr<tmx::TileSet>> tilesets_;
};

}  // namespace tmx

#endif  // STP_TILEMAP_HPP
