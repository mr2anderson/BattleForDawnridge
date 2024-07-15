/*
 *  Battle for Dawnridge
 *  Copyright (C) 2024 mr2anderson
 *
 *  Battle for Dawnridge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Battle for Dawnridge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Battle for Dawnridge.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Image.hpp"
#include "Textures.hpp"



static uint32_t GET_TEXTURE_W(const std::string& imageName, std::optional<sf::IntRect> rect) {
    if (rect.has_value()) {
        return rect.value().width;
    }
    return Textures::get()->get(imageName)->getSize().x;
}
static uint32_t GET_TEXTURE_H(const std::string& imageName, std::optional<sf::IntRect> rect) {
    if (rect.has_value()) {
        return rect.value().height;
    }
    return Textures::get()->get(imageName)->getSize().y;
}


static uint32_t GET_TEXTURE_W(const std::string& imageName, std::optional<sf::IntRect> rect, uint32_t fixedH) {
    float scale = (float)fixedH / (float)GET_TEXTURE_H(imageName, rect);
    return scale * GET_TEXTURE_W(imageName, rect);
}


Image::Image() = default;
Image::Image(int32_t x, int32_t y, const std::string &imageName, std::optional<sf::IntRect> rect) :
        RectangularUiElement(x, y, GET_TEXTURE_W(imageName, rect), GET_TEXTURE_H(imageName, rect)) {
    if (rect.has_value()) {
        this->sprite.setTextureRect(rect.value());
    }
    this->sprite.setTexture(*Textures::get()->get(imageName));
    this->sprite.setPosition(this->getX(), this->getY());
}
Image::Image(int32_t x, int32_t y, uint32_t h, const std::string &imageName, std::optional<sf::IntRect> rect) :
        RectangularUiElement(x, y, GET_TEXTURE_W(imageName, rect, h), h) {
    float scale = (float)h / (float)GET_TEXTURE_H(imageName, rect);
    if (rect.has_value()) {
        this->sprite.setTextureRect(rect.value());
    }
    this->sprite.setTexture(*Textures::get()->get(imageName));
    this->sprite.setScale(scale, scale);
    this->sprite.setPosition(this->getX(), this->getY());
}
void Image::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    this->RectangularUiElement::draw(target, states);
    
    target.draw(sprite, states);
}
void Image::setX(int32_t newX) {
    this->RectangularUiElement::setX(newX);
    this->rerenderBasedOnPosition();
}
void Image::setY(int32_t newY) {
    this->RectangularUiElement::setY(newY);
    this->rerenderBasedOnPosition();
}
void Image::rerenderBasedOnPosition() {
    this->sprite.setPosition(this->getX(), this->getY());
}