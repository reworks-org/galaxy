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

#ifndef STP_PARSER_HPP
#define STP_PARSER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <vector>
#include <string>

#include "re/deps/pugixml/pugixml.hpp"

#include "../Config.hpp"
#include "Properties.hpp"
#include "TileMap.hpp"
#include "TileSet.hpp"
#include "Layer.hpp"
#include "ObjectGroup.hpp"
#include "ImageLayer.hpp"

namespace tmx {

class Parser {
 private:
    static std::string DecompressString(const std::string& compressed_string);
    static void AddTileToLayer(tmx::Layer* layer, int gid, sf::Vector2i tile_pos, tmx::TileMap* tilemap);
    static tmx::Image ParseImage(const pugi::xml_node& image_node, const std::string& working_dir);

 public:
    static tmx::TileSet* ParseTileSet(pugi::xml_node& tileset_node, const std::string& working_dir);
    static tmx::Layer* ParseLayer(const pugi::xml_node& layer_node, tmx::TileMap* tilemap);
    static tmx::ObjectGroup* ParseObjectGroup(const pugi::xml_node& object_group_node, tmx::TileMap* tilemap);
    static tmx::ImageLayer* ParseImageLayer(const pugi::xml_node& imagelayer_node, const std::string& working_dir);
    static void ParseProperties(const pugi::xml_node& object_node, tmx::Properties* object);
};

}  // namespace tmx

#endif  // STP_PARSER_HPP
