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

#include "TileSet.hpp"

#include <stdexcept>
#include <cmath>
#include <string>

namespace tmx {

TileSet::TileSet() {}

TileSet::TileSet(unsigned int firstgid, const std::string& name, unsigned int tilewidth,
                 unsigned int tileheight, tmx::Image image, unsigned int spacing,
                 unsigned int margin, sf::Vector2i tileoffset) :
        firstgid_(firstgid),
        name_(name),
        tilewidth_(tilewidth),
        tileheight_(tileheight),
        spacing_(spacing),
        margin_(margin),
        width_no_spacing_(0),
        height_no_spacing_(0),
        image_(image),
        tileoffset_(tileoffset) {
    unsigned int width_no_margin = image_.GetWidth() - (margin_ * 2);
    unsigned int height_no_margin =  image_.GetHeight() - (margin_ * 2);
    if (spacing != 0) {
        for (unsigned int i = 0; i <= width_no_margin;) {
            i += tilewidth + spacing_;
            width_no_spacing_ += tilewidth;
        }
        for (unsigned int i = 0; i <= height_no_margin;) {
            i += tileheight + spacing_;
            height_no_spacing_ += tileheight;
        }
    } else {
        width_no_spacing_ = width_no_margin;
        height_no_spacing_ = height_no_margin;
    }
    lastgid_ = firstgid + (width_no_spacing_ / tilewidth) * (height_no_spacing_ / tileheight) - 1;

    // Add each tile to the TileSet
    unsigned int tile_amount = lastgid_ - firstgid_ + 1;
    sf::Vector2u cont;
    sf::Vector2u tile_pos;
    tiles_.reserve(tile_amount);
    for (unsigned int i = 0; i < tile_amount; ++i) {
        tile_pos.x = (cont.x * tilewidth_) + (spacing_ * cont.x) + margin_;
        tile_pos.y = (cont.y * tileheight_) + (spacing_ * cont.y) + margin_;
        sf::IntRect texture_rect(tile_pos.x, tile_pos.y, tilewidth_, tileheight_);

        tiles_.push_back(tmx::TileSet::Tile(i, texture_rect, this));

        cont.x = (cont.x + 1) % (width_no_spacing_ / tilewidth_);
        if (cont.x == 0) cont.y += 1;
    }
}

tmx::TileSet::Tile& TileSet::GetTile(unsigned int id) {
    if (id >= tiles_.size()) {
        char error[100];
        sprintf(error, "Error: tile local id %u out of range.\n", id);
        throw std::out_of_range(error);
    }
    return tiles_[id];
}

sf::IntRect TileSet::GetTextureRect(unsigned int id) const {
    if (id >= tiles_.size()) {
        char error[100];
        sprintf(error, "Error: tile local id %u out of range.\n", id);
        throw std::out_of_range(error);
    }
    return tiles_[id].GetTextureRect();
}

const sf::Texture* TileSet::GetTexture() const {
    return image_.GetTexture();
}

const std::string& TileSet::GetName() const {
    return name_;
}

const sf::Vector2i TileSet::GetTileOffSet() const {
    return tileoffset_;
}

unsigned int TileSet::GetTileWidth() const {
    return tilewidth_;
}

unsigned int TileSet::GetTileHeight() const {
    return tileheight_;
}

unsigned int TileSet::GetFirstGID() const {
    return firstgid_;
}

unsigned int TileSet::GetLastGID() const {
    return lastgid_;
}

////////////////////////////////////////////////////////////
// TileSet::Tile implementation
////////////////////////////////////////////////////////////

TileSet::Tile::Tile(unsigned int id, sf::IntRect texture_rect, const tmx::TileSet* parent) :
        id_(id),
        parent_(parent),
        texture_rect_(texture_rect) {
}

const sf::Texture* TileSet::Tile::GetTexture() const {
    return parent_->GetTexture();
}

sf::IntRect TileSet::Tile::GetTextureRect() const {
    return texture_rect_;
}

}  // namespace tmx
