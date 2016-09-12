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

#include "ImageLayer.hpp"

#include <string>

#include <SFML/Graphics/RenderTarget.hpp>

namespace tmx {

ImageLayer::ImageLayer() {}

ImageLayer::ImageLayer(const std::string& name, unsigned int width,
                       unsigned int height, float opacity, bool visible, tmx::Image image) :
        MapObject(name, width, height, opacity, visible),
        image_(image) {
    SetOpacity(opacity);

    float texture_size_x = static_cast<float>(image.GetTexture()->getSize().x);
    float texture_size_y = static_cast<float>(image.GetTexture()->getSize().y);

    vertices_[0].position = sf::Vector2f(0.f, 0.f);
    vertices_[1].position = sf::Vector2f(texture_size_x, 0.f);
    vertices_[2].position = sf::Vector2f(texture_size_x, texture_size_y);
    vertices_[3].position = sf::Vector2f(0.f, texture_size_y);

    vertices_[0].texCoords = sf::Vector2f(0.f, 0.f);
    vertices_[1].texCoords = sf::Vector2f(texture_size_x, 0.f);
    vertices_[2].texCoords = sf::Vector2f(texture_size_x, texture_size_y);
    vertices_[3].texCoords = sf::Vector2f(0.f, texture_size_y);
}

const sf::Texture* ImageLayer::GetTexture() const {
    return image_.GetTexture();
}

void ImageLayer::SetColor(const sf::Color& color) {
    color_.r = color.r;
    color_.g = color.g;
    color_.b = color.b;
    vertices_[0].color = color_;
    vertices_[1].color = color_;
    vertices_[2].color = color_;
    vertices_[3].color = color_;
}

void ImageLayer::SetOpacity(float opacity) {
    color_.a = static_cast<unsigned char>(255 * opacity);
    vertices_[0].color = color_;
    vertices_[1].color = color_;
    vertices_[2].color = color_;
    vertices_[3].color = color_;
}

void ImageLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (visible) {
        states.texture = image_.GetTexture();
        target.draw(vertices_, 4, sf::Quads, states);
    }
}

}  // namespace tmx
