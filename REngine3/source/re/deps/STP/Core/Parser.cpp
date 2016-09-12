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

#include "Parser.hpp"

#include <cstring>

#include <string>
#include <sstream>
#include <vector>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <zlib.h>

#include "Base64.hpp"

const unsigned int FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned int FLIPPED_VERTICALLY_FLAG = 0x40000000;
const unsigned int FLIPPED_DIAGONALLY_FLAG = 0x20000000;

namespace tmx {

std::string Parser::DecompressString(const std::string& compressed_string) {
    z_stream zstream;

    std::string outstring;

    zstream.zalloc = Z_NULL;
    zstream.zfree = Z_NULL;
    zstream.opaque = Z_NULL;
    zstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(compressed_string.data()));
    zstream.avail_in = compressed_string.size();

    int result;
    result = inflateInit2(&zstream, 15 + 32);

	char outbuffer[32768];

    if (result != Z_OK) {
        return NULL;
    }

    do {
        zstream.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zstream.avail_out = sizeof(outbuffer);

        result = inflate(&zstream, Z_SYNC_FLUSH);

        switch (result) {
            case Z_NEED_DICT:
            case Z_STREAM_ERROR:
                result = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&zstream);
                return NULL;
        }

        if (outstring.size() < zstream.total_out) {
            outstring.append(outbuffer, zstream.total_out - outstring.size());
        }
    } while (result != Z_STREAM_END);

    if (zstream.avail_in != 0) {
        return NULL;
    }

    inflateEnd(&zstream);

    return outstring;
}

void Parser::ParseProperties(const pugi::xml_node& object_node, tmx::Properties* object) {
    pugi::xml_node properties_node = object_node.child("properties");
    if (properties_node) {
        for (const pugi::xml_node& property_node : properties_node.children("property")) {
            std::string name = property_node.attribute("name").as_string();
            std::string value = property_node.attribute("value").as_string();
            object->AddProperty(name, value);
        }
    }
}

void Parser::AddTileToLayer(tmx::Layer* layer, int gid, sf::Vector2i tile_pos, tmx::TileMap* tilemap) {
    tmx::TileSet* tileset = tilemap->GetTileSet(gid);

    if (tileset != nullptr) {
        tile_pos.x += tileset->GetTileOffSet().x;
        tile_pos.y += tileset->GetTileOffSet().y;
        sf::IntRect tile_rect(tile_pos.x, tile_pos.y, tilemap->GetTileWidth(), tilemap->GetTileHeight());
        layer->AddTile(gid, tile_rect, tileset);
    } else {
        layer->AddTile(gid, sf::IntRect(tile_pos.x, tile_pos.y, 0, 0));
    }
}

tmx::Image Parser::ParseImage(const pugi::xml_node& image_node, const std::string& working_dir) {
    std::string format, source;
    unsigned int width = 0, height = 0;
    int32_t trans = -1;

    source = working_dir + image_node.attribute("source").as_string();

    // Check if some attributes exists in image_node
    pugi::xml_attribute attribute_width = image_node.attribute("width");
    pugi::xml_attribute attribute_height = image_node.attribute("height");
    pugi::xml_attribute attribute_trans = image_node.attribute("trans");
    pugi::xml_attribute attribute_format = image_node.attribute("format");

    if (attribute_width) width = attribute_width.as_uint();
    if (attribute_height) height = attribute_height.as_uint();
    if (attribute_trans) {
        std::stringstream ss(attribute_trans.as_string());
        ss >> std::hex >> trans;
    }
    if (attribute_format) format = attribute_format.as_string();

    return tmx::Image(source, width, height, trans, format);
}

tmx::TileSet* Parser::ParseTileSet(pugi::xml_node& tileset_node, const std::string& working_dir) {
    unsigned int firstgid, tilewidth, tileheight, spacing = 0, margin = 0;
    std::string name;
    tmx::Image image_data;
    sf::Vector2i tileoffset_data = {0, 0};

    pugi::xml_node& tileset_node_ = tileset_node;

    firstgid = tileset_node_.attribute("firstgid").as_uint();

    // If it finds a source attribute, load the file and get the tileset
    pugi::xml_document tsx_file;
    pugi::xml_attribute attribute_source = tileset_node_.attribute("source");
    if (attribute_source) {
        std::string source = working_dir + attribute_source.as_string();
        if (!tsx_file.load_file(source.c_str())) {
            fprintf(stdout, "Error loading the XML document.\n");
        }
        tileset_node_ = tsx_file.child("tileset");
    }

    // Get the map data
    name = tileset_node_.attribute("name").as_string();
    tilewidth = tileset_node_.attribute("tilewidth").as_uint();
    tileheight = tileset_node_.attribute("tileheight").as_uint();

    // Check if some attributes exists in tileset_node_
    pugi::xml_attribute attribute_spacing = tileset_node_.attribute("spacing");
    pugi::xml_attribute attribute_margin = tileset_node_.attribute("margin");

    if (attribute_spacing) spacing = attribute_spacing.as_uint();
    if (attribute_margin) margin = attribute_margin.as_uint();

    for (const pugi::xml_node& node : tileset_node.children()) {
        const char* node_name = node.name();
        // Call the respective parse function for each node
        if (strcmp(node_name, "tileoffset") == 0) {
            tileoffset_data.x = node.attribute("x").as_int();
            tileoffset_data.y = node.attribute("y").as_int();
        } else if (strcmp(node_name, "image") == 0) {
            image_data = ParseImage(node, working_dir);
        } else if (strcmp(node_name, "terraintypes") == 0) {
        }
    }

    // Create the new TileSet
    tmx::TileSet* tileset = new tmx::TileSet(firstgid, name, tilewidth, tileheight,
                                             image_data, spacing, margin, tileoffset_data);

    // Parse each tile property
    for (const pugi::xml_node& tile_node : tileset_node_.children("tile")) {
        unsigned int id = tile_node.attribute("id").as_uint();
        ParseProperties(tile_node, &tileset->GetTile(id));
    }

    // Parse the tileset properties
    ParseProperties(tileset_node_, tileset);

    return tileset;
}

tmx::Layer* Parser::ParseLayer(const pugi::xml_node& layer_node, tmx::TileMap* tilemap) {
    std::string name;
    unsigned int width, height;
    float opacity = 1.f;  // range 0 - 1
    bool visible = true;

    unsigned int count_x = 0;
    unsigned int count_y = 0;

    unsigned int tilewidth = tilemap->GetTileWidth();
    unsigned int tileheight = tilemap->GetTileHeight();

    sf::Vector2i tile_pos;

    // Get the map data
    name = layer_node.attribute("name").as_string();
    width = layer_node.attribute("width").as_uint();
    height = layer_node.attribute("height").as_uint();

    // Check if some attributes exists in layer_node
    pugi::xml_attribute attribute_opacity = layer_node.attribute("opacity");
    pugi::xml_attribute attribute_visible = layer_node.attribute("visible");

    if (attribute_opacity) opacity = attribute_opacity.as_float();
    if (attribute_visible) visible = attribute_visible.as_bool();

    // Create the new Layer
    tmx::Layer* layer = new tmx::Layer(name, width, height, opacity, visible, tilemap->GetOrientation());

    // Parse the layer properties
    Parser::ParseProperties(layer_node, layer);

    // Parse the layer data
    pugi::xml_node data_node = layer_node.child("data");
    if (data_node) {
        std::string data = data_node.text().as_string();
        // Check if the encoding attribute exists in data_node
        pugi::xml_attribute attribute_encoding = data_node.attribute("encoding");
        if (attribute_encoding) {
            std::string encoding = attribute_encoding.as_string();

            if (encoding == "base64") {  // Base64 encoding
                std::stringstream ss;
                ss << data;
                ss >> data;
                data = base64_decode(data);

                int expectedSize = width * height * 4;  // number of tiles * 4 bytes = 32bits / tile
                std::vector<unsigned char>byteVector;  // to hold decompressed data as bytes
                byteVector.reserve(expectedSize);

                // Check if the compression attribute exists in data_node
                if (data_node.attribute("compression")) {
                    std::string decompressed_data = DecompressString(data);
                    for (std::string::iterator i = decompressed_data.begin(); i != decompressed_data.end(); ++i)
                        byteVector.push_back(*i);
                } else {
                    for (std::string::iterator i = data.begin(); i != data.end(); ++i)
                        byteVector.push_back(*i);
                }

                for (unsigned int i = 0; i < byteVector.size() - 3 ; i += 4) {
                    unsigned int gid = byteVector[i] |
                                       byteVector[i + 1] << 8 |
                                       byteVector[i + 2] << 16 |
                                       byteVector[i + 3] << 24;

                    gid &= ~(FLIPPED_HORIZONTALLY_FLAG |
                             FLIPPED_VERTICALLY_FLAG |
                             FLIPPED_DIAGONALLY_FLAG);

                    tile_pos = sf::Vector2i(count_x * tilewidth, count_y * tileheight);

                    AddTileToLayer(layer, gid, tile_pos, tilemap);

                    count_x = (count_x + 1) % width;
                    if (count_x == 0) count_y += 1;
                }
            } else if (encoding == "csv") {  // CSV encoding
                std::stringstream data_stream(data);
                unsigned int gid;
                while (data_stream >> gid) {
                    if (data_stream.peek() == ',')
                        data_stream.ignore();

                    gid &= ~(FLIPPED_HORIZONTALLY_FLAG |
                             FLIPPED_VERTICALLY_FLAG |
                             FLIPPED_DIAGONALLY_FLAG);

                    tile_pos = sf::Vector2i(count_x * tilewidth, count_y * tileheight);

                    AddTileToLayer(layer, gid, tile_pos, tilemap);

                    count_x = (count_x + 1) % width;
                    if (count_x == 0) count_y += 1;
                }
            }
        } else {  // Unencoded
            for (const pugi::xml_node& tile_node : data_node.children("tile")) {
                unsigned int gid = tile_node.attribute("gid").as_uint();

                gid &= ~(FLIPPED_HORIZONTALLY_FLAG |
                         FLIPPED_VERTICALLY_FLAG |
                         FLIPPED_DIAGONALLY_FLAG);

                tile_pos = sf::Vector2i(count_x * tilewidth, count_y * tileheight);

                AddTileToLayer(layer, gid, tile_pos, tilemap);

                count_x = (count_x + 1) % width;
                if (count_x == 0) count_y += 1;
            }
        }
    }

    return layer;
}

tmx::ObjectGroup* Parser::ParseObjectGroup(const pugi::xml_node& object_group_node, tmx::TileMap* tilemap) {
    std::string name;
    int32_t color = -1;
    unsigned int width, height;
    float opacity = 1.f;
    bool visible = true;

    name = object_group_node.attribute("name").as_string();
    width = object_group_node.attribute("width").as_uint();
    height = object_group_node.attribute("height").as_uint();

    // Check if some attributes exists in object_group_node
    pugi::xml_attribute attribute_color = object_group_node.attribute("color");
    pugi::xml_attribute attribute_opacity = object_group_node.attribute("opacity");
    pugi::xml_attribute attribute_visible = object_group_node.attribute("visible");

    if (attribute_color) {
        std::string hex_string(attribute_color.as_string());
        hex_string.erase(0, 1);  // Deletes the '#' character in the string
        std::stringstream ss;

        ss << hex_string;
        ss >> std::hex >> color;
    }
    if (attribute_opacity) opacity = attribute_opacity.as_float();
    if (attribute_visible) visible = attribute_visible.as_bool();

    // Create the new ObjectGroup
    tmx::ObjectGroup* object_group = new tmx::ObjectGroup(name, width, height, opacity,
                                                          visible, color);

    // Parse the objectgroup properties
    Parser::ParseProperties(object_group_node, object_group);

    // Parse each objectgroup object
    for (const pugi::xml_node& object_node : object_group_node.children("object")) {
        std::string object_name;
        std::string object_type;
        int object_x, object_y;
        unsigned int object_width = 0, object_height = 0;
        float object_rotation = 0;
        bool object_visible = true;

        object_x = object_node.attribute("x").as_int();
        object_y = object_node.attribute("y").as_int();

        // Check if some attributes exists in object_node
        pugi::xml_attribute attribute_name = object_node.attribute("name");
        pugi::xml_attribute attribute_type = object_node.attribute("type");
        pugi::xml_attribute attribute_width = object_node.attribute("width");
        pugi::xml_attribute attribute_height = object_node.attribute("height");
        pugi::xml_attribute attribute_rotation = object_node.attribute("rotation");
        pugi::xml_attribute attribute_visible = object_node.attribute("visible");
        pugi::xml_attribute attribute_gid = object_node.attribute("gid");

        if (attribute_name) object_name = attribute_name.as_string();
        if (attribute_type) object_type = attribute_type.as_string();
        if (attribute_width) object_width = attribute_width.as_uint();
        if (attribute_height) object_height = attribute_height.as_uint();
        if (attribute_rotation) object_rotation = attribute_rotation.as_float();
        if (attribute_visible) object_visible = attribute_visible.as_bool();

        if (attribute_gid) {
            // Tile Object
            unsigned int gid = attribute_gid.as_uint();  // Tile global id

            gid &= ~(FLIPPED_HORIZONTALLY_FLAG |
                     FLIPPED_VERTICALLY_FLAG |
                     FLIPPED_DIAGONALLY_FLAG);

            unsigned int id = gid - tilemap->GetTileSet(gid)->GetFirstGID();  // Tile local id

            tmx::TileSet::Tile* tile = &tilemap->GetTileSet(gid)->GetTile(id);

            tmx::ObjectGroup::Object newobject(object_name, object_type, object_x, object_y,
                                               object_width, object_height, object_rotation,
                                               object_visible, tmx::Tile, "", tile);
            Parser::ParseProperties(object_node, &newobject);
            object_group->AddObject(newobject);
        } else if (object_width && object_height) {
            if (object_node.child("ellipse")) {
                // Ellipse Object
                tmx::ObjectGroup::Object newobject(object_name, object_type, object_x, object_y,
                                                   object_width, object_height, object_rotation,
                                                   object_visible, tmx::Ellipse);
                Parser::ParseProperties(object_node, &newobject);
                object_group->AddObject(newobject);
            } else {
                // Rectangle Object
                tmx::ObjectGroup::Object newobject(object_name, object_type, object_x, object_y,
                                                   object_width, object_height, object_rotation,
                                                   object_visible, tmx::Rectangle);
                Parser::ParseProperties(object_node, &newobject);
                object_group->AddObject(newobject);
            }
        } else {
            pugi::xml_node polygon_node = object_node.child("polygon");
            pugi::xml_node polyline_node = object_node.child("polyline");
            if (polygon_node) {
                // Polygon Object
                std::string vertices_points = polygon_node.attribute("points").as_string();
                tmx::ObjectGroup::Object newobject(object_name, object_type, object_x, object_y,
                                                   object_width, object_height, object_rotation,
                                                   object_visible, tmx::Polygon, vertices_points);
                Parser::ParseProperties(object_node, &newobject);
                object_group->AddObject(newobject);
            } else if (polyline_node) {
                // Polyline Object
                std::string vertices_points = polyline_node.attribute("points").as_string();
                tmx::ObjectGroup::Object newobject(object_name, object_type, object_x, object_y,
                                                   object_width, object_height, object_rotation,
                                                   object_visible, tmx::Polyline, vertices_points);
                Parser::ParseProperties(object_node, &newobject);
                object_group->AddObject(newobject);
            }
        }
    }

    return object_group;
}

tmx::ImageLayer* Parser::ParseImageLayer(const pugi::xml_node& imagelayer_node, const std::string& working_dir) {
    std::string name;
    unsigned int width, height;
    float opacity = 1.f;  // range 0 - 1
    bool visible = true;
    tmx::Image image_data;

    // Get the map data
    name = imagelayer_node.attribute("name").as_string();
    width = imagelayer_node.attribute("width").as_uint();
    height = imagelayer_node.attribute("height").as_uint();

    // Check if some attributes exists in imagelayer_node
    pugi::xml_attribute attribute_opacity = imagelayer_node.attribute("opacity");
    pugi::xml_attribute attribute_visible = imagelayer_node.attribute("visible");

    if (attribute_opacity) opacity = attribute_opacity.as_float();
    if (attribute_visible) visible = attribute_visible.as_bool();

    // Parse the image data
    pugi::xml_node image_node = imagelayer_node.child("image");
    if (image_node) image_data = ParseImage(image_node, working_dir);

    // Create the new ImageLayer
    tmx::ImageLayer* imagelayer = new tmx::ImageLayer(name, width, height, opacity, visible, image_data);

    // Parse the imagelayer properties
    Parser::ParseProperties(imagelayer_node, imagelayer);

    return imagelayer;
}

}  // namespace tmx
