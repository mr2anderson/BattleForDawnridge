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


Image::Image() = default;
Image::Image(int32_t x, int32_t y, const std::string& textureName, std::optional<sf::IntRect> rect) :
        RectangularUiElement(x, y, GET_TEXTURE_W(textureName, rect), GET_TEXTURE_H(textureName, rect)) {
    this->textureName = textureName;
    this->textureRect = rect;
}
Image::Image(int32_t x, int32_t y, uint32_t size, const std::string& textureName, std::optional<sf::IntRect> rect) :
        RectangularUiElement(x, y, size, size) {
    this->textureName = textureName;
    this->textureRect = rect;

    this->scaleX = (float)size / GET_TEXTURE_W(textureName, rect);
    this->scaleY = (float)size / GET_TEXTURE_H(textureName, rect);

    if (this->scaleX >= 1) {
        float dw = GET_TEXTURE_W(textureName, rect) * (this->scaleX.value() - 1);
        this->dPosX = dw / 2;
        this->scaleX = 1;
    }
    else {
        this->dPosX = 0;
    }

    if (scaleY >= 1) {
        float dh = GET_TEXTURE_H(textureName, rect) * (this->scaleY.value() - 1);
        this->dPosY = dh / 2;
        this->scaleY = 1;
    }
    else {
        this->dPosY = 0;
    }
}
void Image::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    this->RectangularUiElement::draw(target, states);
    
    sf::Sprite sprite;
    sprite.setTexture(*Textures::get()->get(this->textureName));
    sprite.setPosition(this->getX() + this->dPosX.value_or(0), this->getY() + this->dPosY.value_or(0));
    if (this->textureRect.has_value()) {
        sprite.setTextureRect(this->textureRect.value());
    }
    sprite.setScale(this->scaleX.value_or(1), this->scaleY.value_or(1));
    target.draw(sprite, states);
}