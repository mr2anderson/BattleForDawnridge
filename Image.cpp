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


Image::Image() = default;
Image::Image(int32_t x, int32_t y, const std::string &imageName) :
        RectangularUiElement(x, y, Textures::get()->get(imageName)->getSize().x, Textures::get()->get(imageName)->getSize().y) {
    this->sprite.setTexture(*Textures::get()->get(imageName));
    this->sprite.setPosition(this->getX(), this->getY());
}
Image::Image(int32_t x, int32_t y, uint32_t h, const std::string &imageName) :
        RectangularUiElement(x, y, (float)Textures::get()->get(imageName)->getSize().x * (float)h / (float)Textures::get()->get(imageName)->getSize().y, h) {
    float scale = (float)h / (float)Textures::get()->get(imageName)->getSize().y;
    this->sprite.setTexture(*Textures::get()->get(imageName));
    this->sprite.setScale(scale, scale);
    this->sprite.setPosition(this->getX(), this->getY());
}
Image::Image(int32_t x, int32_t y, uint32_t w, uint32_t h, bool left, bool up, const std::string& imageName) :
        RectangularUiElement(x, y, w, h) {
    this->sprite.setTexture(*Textures::get()->get(imageName));
    uint32_t textureWidth = this->sprite.getTexture()->getSize().x;
    uint32_t textureHeight = this->sprite.getTexture()->getSize().y;
    if (w > textureWidth) {
        w = textureWidth;
    }
    if (h > textureHeight) {
        h = textureHeight;
    }
    if (left) {
        if (up) {
            this->sprite.setTextureRect(sf::IntRect(0, 0, w, h));
        }
        else {
            this->sprite.setTextureRect(sf::IntRect(0, textureHeight - h, w, h));
        }
    }
    else {
        if (up) {
            this->sprite.setTextureRect(sf::IntRect(textureWidth - w, 0, w, h));
        }
        else {
            this->sprite.setTextureRect(sf::IntRect(textureWidth - w, textureHeight - h, w, h));
        }
    }
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