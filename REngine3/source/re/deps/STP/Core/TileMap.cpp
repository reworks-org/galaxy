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

#include "TileMap.hpp"

#include <cstdio>
#include <string>

#include "re/deps/pugixml/pugixml.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

#include "Parser.hpp"

namespace tmx {

TileMap::TileMap(const std::string& file_to_parse) {
    pugi::xml_document tmx_file;

    if (!tmx_file.load_file(file_to_parse.c_str())) {
        fprintf(stdout, "Error loading the XML document.\n");
    }

    pugi::xml_node map_node;

    if (!(map_node = tmx_file.child("map"))) {
        fprintf(stdout, "The document is not a valid TMX file.\n");
    }

    working_dir_ = file_to_parse.substr(0, file_to_parse.find_last_of('/') + 1);

    // Get the map data
    version_ = map_node.attribute("version").as_float();
    orientation_ = map_node.attribute("orientation").value();
    width_ = map_node.attribute("width").as_uint();
    height_ = map_node.attribute("height").as_uint();
    tilewidth_ = map_node.attribute("tilewidth").as_uint();
    tileheight_ = map_node.attribute("tileheight").as_uint();

    for (pugi::xml_node node : map_node.children()) {
        std::string node_name = node.name();
        // Call the respective parse function for each node
        if (node_name == "tileset") {
            AddTileSet(Parser::ParseTileSet(node, working_dir_));
        } else if (node_name == "layer") {
            AddLayer(Parser::ParseLayer(node, this));  // Receive this to get the tiles stored in the TileSets
        } else if (node_name == "objectgroup") {
            AddObjectGroup(Parser::ParseObjectGroup(node, this));  // Receive this to get the tiles stored in the TileSets
        } else if (node_name == "imagelayer") {
            AddImageLayer(Parser::ParseImageLayer(node, working_dir_));
        }
    }

    // Parse the map properties
    Parser::ParseProperties(map_node, this);

    ShowObjects(false);
}

void TileMap::AddLayer(tmx::Layer* newlayer) {
    map_objects_.push_back(std::unique_ptr<tmx::MapObject>(newlayer));
    layers_[newlayer->GetName()] = newlayer;
}

void TileMap::AddObjectGroup(tmx::ObjectGroup* newobjectgroup) {
    map_objects_.push_back(std::unique_ptr<tmx::MapObject>(newobjectgroup));
    object_groups_[newobjectgroup->GetName()] = newobjectgroup;
}

void TileMap::AddImageLayer(tmx::ImageLayer* newimagelayer) {
    map_objects_.push_back(std::unique_ptr<tmx::MapObject>(newimagelayer));
    image_layers_[newimagelayer->GetName()] = newimagelayer;
}

void TileMap::AddTileSet(tmx::TileSet* newtileset) {
    tilesets_.push_back(std::unique_ptr<tmx::TileSet>(newtileset));
    tilesets_hash_[newtileset->GetName()] = newtileset;
}

tmx::TileSet* TileMap::GetTileSet(unsigned int gid) {
    if (gid == 0) return nullptr;
    for (unsigned int i = 0; i < tilesets_.size(); ++i) {
        if (gid >= tilesets_[i]->GetFirstGID() && gid <= tilesets_[i]->GetLastGID())
            return &(*tilesets_[i]);
    }
    return nullptr;
}

tmx::TileSet& TileMap::GetTileSet(const std::string& tileset_name) {
    return *tilesets_hash_[tileset_name];
}

tmx::Layer& TileMap::GetLayer(const std::string& layer_name) {
    return *layers_[layer_name];
}

tmx::ObjectGroup& TileMap::GetObjectGroup(const std::string& objectgroup_name) {
    return *object_groups_[objectgroup_name];
}

tmx::ImageLayer& TileMap::GetImageLayer(const std::string& imagelayer_name) {
    return *image_layers_[imagelayer_name];
}

void TileMap::ShowObjects(bool show) {
    for (auto& object_group : object_groups_)
        object_group.second->visible = show;
}

unsigned int TileMap::GetWidth() const {
    return width_;
}

unsigned int TileMap::GetHeight() const {
    return height_;
}

unsigned int TileMap::GetTileWidth() const {
    return tilewidth_;
}

unsigned int TileMap::GetTileHeight() const {
    return tileheight_;
}

const std::string& TileMap::GetOrientation() const {
    return orientation_;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates /* states */) const {
    for (unsigned int i = 0; i < map_objects_.size(); ++i) {
        if (map_objects_[i]->visible == true)
            target.draw(*map_objects_[i]);
    }
}

}  // namespace tmx
