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

#include "Layer.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

namespace tmx {

Layer::Layer() {}

Layer::Layer(const std::string& name, unsigned int width,
             unsigned int height, float opacity, bool visible, std::string orientation) :
        MapObject(name, width, height, opacity, visible),
        orientation_(orientation) {
    // Reserve space for each vector to avoid reallocate
    tiles_.reserve(width * height);
    unsigned char alpha = static_cast<unsigned char>(255 * opacity);
    color_.a = alpha;
}

void Layer::AddTile(unsigned int gid, sf::IntRect tile_rect, tmx::TileSet* tileset) {
    tiles_.emplace_back(gid, tile_rect, orientation_, tileset);
	tiles_.back().SetColor(color_);
}

unsigned int Layer::GetWidth() const {
    return width_;
}

unsigned int Layer::GetHeight() const {
    return height_;
}

tmx::Layer::Tile& Layer::GetTile(unsigned int x, unsigned int y) {
    if (x >= width_ || y >= height_) {
        char error[100];
        sprintf(error, "Error: tile (%u, %u) out of range.\n", x, y);
        throw std::out_of_range(error);
    }

    unsigned int pos = (y * width_) + x;

    return tiles_[pos];
}

void Layer::SetColor(const sf::Color& color) {
    color_.r = color.r;
    color_.g = color.g;
    color_.b = color.b;
    for (auto& tile : tiles_) {
        tile.SetColor(color_);
    }
}

void Layer::SetOpacity(float opacity) {
    color_.a = static_cast<unsigned char>(255 * opacity);
    for (auto& tile : tiles_) {
        tile.SetColor(color_);
    }
}

void Layer::draw(sf::RenderTarget& target, sf::RenderStates /* states */) const {
    if (visible) {
        for (unsigned int i = 0; i < tiles_.size(); ++i)
            target.draw(tiles_[i]);
    }
}

////////////////////////////////////////////////////////////
// Layer::Tile implementation
////////////////////////////////////////////////////////////

Layer::Tile::Tile() {}

Layer::Tile::Tile(unsigned int gid, sf::IntRect tile_rect, std::string orientation, tmx::TileSet* tileset) :
        gid_(gid),
        tile_rect_(tile_rect),
        tile_properties_(nullptr),
        texture_(nullptr),
        texture_rect_(),
        visible(true) {
    if (tileset) {
        unsigned int id = gid - tileset->GetFirstGID();  // The local id of the tile in the tileset

        texture_ = tileset->GetTexture();
        texture_rect_ = tileset->GetTextureRect(id);
        tile_properties_ = &tileset->GetTile(id);
    }

    if (orientation != "orthogonal" && tile_rect.width != 0 && tile_rect_.height != 0) {
    	int x = tile_rect_.left / tile_rect_.width;
        int y = tile_rect_.top / tile_rect_.height;

    	if (orientation == "isometric") {
    	    tile_rect_.left = (x-y) * tile_rect_.width * 0.5;
            tile_rect_.top = (x+y) * tile_rect_.height * 0.5;
        }

        if (orientation == "staggered") {
            if ((y % 2) == 0) {
                tile_rect_.left = x * tile_rect_.width;
            }
            else {
                tile_rect_.left = x * tile_rect_.width + tile_rect_.width /2;
            }
            tile_rect_.top = y * tile_rect_.height / 2;
        }
    }
    else if (tile_rect_.width == 0 || tile_rect_.height == 0)
    {
        tile_rect_.width = texture_rect_.width;
        tile_rect_.height = texture_rect_.height;
    }

    UpdatePositions();
    UpdateTexCoords();
}

void Layer::Tile::UpdatePositions() {
    sf::FloatRect bounds = GetGlobalBounds();

    vertices_[0].position = sf::Vector2f(bounds.left, bounds.top);
    vertices_[1].position = sf::Vector2f(bounds.left + texture_rect_.width, bounds.top);
    vertices_[2].position = sf::Vector2f(bounds.left + texture_rect_.width, bounds.top + texture_rect_.height);
    vertices_[3].position = sf::Vector2f(bounds.left, bounds.top + texture_rect_.height);
}

void Layer::Tile::UpdateTexCoords() {
    float left   = static_cast<float>(texture_rect_.left);
    float right  = left + texture_rect_.width;
    float top    = static_cast<float>(texture_rect_.top);
    float bottom = top + texture_rect_.height;

    vertices_[0].texCoords = sf::Vector2f(left, top);
    vertices_[1].texCoords = sf::Vector2f(right, top);
    vertices_[2].texCoords = sf::Vector2f(right, bottom);
    vertices_[3].texCoords = sf::Vector2f(left, bottom);
}

sf::FloatRect Layer::Tile::GetGlobalBounds() const {
    float left = static_cast<float>(tile_rect_.left);
    float top = static_cast<float>(tile_rect_.top);
    float width = static_cast<float>(tile_rect_.width);
    float height = static_cast<float>(tile_rect_.height);

    return sf::FloatRect(left, top, width, height);
}

bool Layer::Tile::empty() const {
    if (gid_ == 0 || !texture_) return true;
    else return false;
}

void Layer::Tile::SetColor(const sf::Color& color) {
    vertices_[0].color = color;
    vertices_[1].color = color;
    vertices_[2].color = color;
    vertices_[3].color = color;
}

void Layer::Tile::AddProperty(const std::string& name, const std::string& value) {
    tile_properties_->AddProperty(name, value);
}

std::string& Layer::Tile::GetPropertyValue(const std::string& name) {
    return tile_properties_->GetPropertyValue(name);
}

void Layer::Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (texture_ && visible) {
        states.texture = texture_;
        target.draw(vertices_, 4, sf::Quads, states);
    }
}

}  // namespace tmx
