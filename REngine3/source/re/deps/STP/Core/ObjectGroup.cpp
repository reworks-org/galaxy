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

#include "ObjectGroup.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <cmath>

#include <SFML/Graphics/RenderTarget.hpp>

namespace tmx {

ObjectGroup::ObjectGroup() {}

ObjectGroup::ObjectGroup(const std::string& name, unsigned int width, unsigned int height,
                         float opacity, bool visible, int32_t hexcolor) :
        MapObject(name, width, height, opacity, visible) {
    if (hexcolor >= 0) {
        color_.r = hexcolor >> 16;
        color_.g = (hexcolor >> 8) & 0xff;
        color_.b = hexcolor & 0xff;
    }
    color_.a = static_cast<unsigned char>(255 * opacity);
}

void ObjectGroup::AddObject(tmx::ObjectGroup::Object newobject) {
    newobject.SetColor(color_);
    objects_.push_back(newobject);
}

void ObjectGroup::SetOpacity(float opacity) {
    color_.a = static_cast<unsigned char>(255 * opacity);
    for (auto& object : objects_) {
        object.SetColor(color_);
    }
}

void ObjectGroup::SetColor(const sf::Color& color) {
    color_.r = color.r;
    color_.g = color.g;
    color_.b = color.b;
    for (auto& object : objects_) {
        object.SetColor(color_);
    }
}

void ObjectGroup::draw(sf::RenderTarget& target, sf::RenderStates /* states */) const {
    if (visible) {
        for (unsigned int i = 0; i < objects_.size(); ++i)
            target.draw(objects_[i]);
    }
}

////////////////////////////////////////////////////////////
// ObjectGroup::Object implementation
////////////////////////////////////////////////////////////

ObjectGroup::Object::Object(const std::string& name, const std::string& type, int x, int y,
               unsigned int width, unsigned int height, float rotation, bool visible,
               tmx::ObjectType shape_type, const std::string& vertices_points, tmx::TileSet::Tile* tile) :
        name_(name),
        type_(type),
        x_(x),
        y_(y),
        width_(width),
        height_(height),
        rotation_(rotation),
        tile_(tile),
        visible(visible) {
    float left = static_cast<float>(x);
    float top = static_cast<float>(y);
    float l_width = static_cast<float>(width);
    float l_height = static_cast<float>(height);

    if (shape_type == tmx::Polygon || shape_type == tmx::Polyline) {
        if (!vertices_points.empty()) {
            std::stringstream sstream(vertices_points);
            float x_pos, y_pos;
            while (sstream >> x_pos) {
                if (sstream.peek() == ',')
                    sstream.ignore();
                sstream >> y_pos;
                vertices_.push_back(sf::Vertex(sf::Vector2f(x_pos + left, y_pos + top)));
            }
            if (shape_type == tmx::Polygon)
                vertices_.push_back(vertices_[0]);  // Add the last vertex to close the polygon
        }
    } else if (shape_type == tmx::Rectangle) {
        vertices_.push_back(sf::Vertex(sf::Vector2f(left, top)));
        vertices_.push_back(sf::Vertex(sf::Vector2f(left + l_width, top)));
        vertices_.push_back(sf::Vertex(sf::Vector2f(left + l_width, top + l_height)));
        vertices_.push_back(sf::Vertex(sf::Vector2f(left, top + l_height)));
        vertices_.push_back(sf::Vertex(sf::Vector2f(left, top)));  // Add the last vertex to close the rectangle
    } else if (shape_type == tmx::Ellipse) {
        const float PI = 3.14159265358979f;
        float a = l_width / 2.f;
        float b = l_height / 2.f;
        float x, y;  // x and y global coordinates
        sf::Vector2f center(left + a, top + b);
        float partitions = pow(2.f, 3.f + ceil((a + b) / 30));
        float angle_increment = 360.f / partitions;
        for (int i = 0; i * angle_increment <= 360.f; ++i) {
            x = center.x + a * cos((i * angle_increment) * PI / 180.f);
            y = center.y + b * sin((i * angle_increment) * PI / 180.f);
            vertices_.push_back(sf::Vertex(sf::Vector2f(x, y)));
        }
    } else if (shape_type == tmx::Tile) {
        sf::FloatRect tile_rect = static_cast<sf::FloatRect>(tile->GetTextureRect());

        vertices_.resize(4);

        vertices_[0].position = sf::Vector2f(left, top);
        vertices_[1].position = sf::Vector2f(left + tile_rect.width, top);
        vertices_[2].position = sf::Vector2f(left + tile_rect.width, top + tile_rect.height);
        vertices_[3].position = sf::Vector2f(left, top + tile_rect.height);

        vertices_[0].texCoords = sf::Vector2f(tile_rect.left, tile_rect.top);
        vertices_[1].texCoords = sf::Vector2f(tile_rect.left + tile_rect.width, tile_rect.top);
        vertices_[2].texCoords = sf::Vector2f(tile_rect.left + tile_rect.height, tile_rect.top + tile_rect.height);
        vertices_[3].texCoords = sf::Vector2f(tile_rect.left, tile_rect.top + tile_rect.height);
    }
}

void ObjectGroup::Object::SetColor(const sf::Color& color) {
    for (auto& vertice : vertices_) {
        vertice.color = color;
    }
}

void ObjectGroup::Object::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (visible) {
        if (tile_) {
            states.texture = tile_->GetTexture();
            target.draw(&vertices_[0], vertices_.size(), sf::Quads, states);
        } else {
            target.draw(&vertices_[0], vertices_.size(), sf::LinesStrip);
        }
    }
}

}  // namespace tmx
