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



static uint32_t GET_TEXTURE_W(const std::string& imageName, boost::optional<IntRectSerializable> rect) {
    if (rect.has_value()) {
        return rect.value().width;
    }
    return Textures::get()->get(imageName)->getSize().x;
}
static uint32_t GET_TEXTURE_H(const std::string& imageName, boost::optional<IntRectSerializable> rect) {
    if (rect.has_value()) {
        return rect.value().height;
    }
    return Textures::get()->get(imageName)->getSize().y;
}


Image::Image() = default;
Image::Image(int32_t x, int32_t y, const std::string& textureName, boost::optional<IntRectSerializable> rect) :
        RectangularUiElement(x, y, GET_TEXTURE_W(textureName, rect), GET_TEXTURE_H(textureName, rect)) {
    this->textureName = textureName;
    this->textureRect = rect;
}
Image::Image(int32_t x, int32_t y, uint32_t size, const std::string& textureName, boost::optional<IntRectSerializable> rect) :
        RectangularUiElement(x, y, size, size) {
    this->textureName = textureName;
    this->size = size;
    this->textureRect = rect;
}
void Image::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    this->RectangularUiElement::draw(target, states);

    float scaleX, scaleY;
    if (this->size.has_value()) {
        scaleX = (float)this->size.value() / GET_TEXTURE_W(textureName, this->textureRect);
        scaleY = (float)this->size.value() / GET_TEXTURE_H(textureName, this->textureRect);
    }
    else {
        scaleX = 1;
        scaleY = 1;
    }
   

    float dPosX;
    if (scaleX >= 1) {
        float dw = GET_TEXTURE_W(textureName, this->textureRect) * (scaleX - 1);
        dPosX = dw / 2;
        scaleX = 1;
    }
    else {
        dPosX = 0;
    }

    float dPosY;
    if (scaleY >= 1) {
        float dh = GET_TEXTURE_H(textureName, this->textureRect) * (scaleY - 1);
        dPosY = dh / 2;
        scaleY = 1;
    }
    else {
        dPosY = 0;
    }
    
    sf::Sprite sprite;
    sprite.setTexture(*Textures::get()->get(this->textureName));
    sprite.setPosition(this->getX() + dPosX, this->getY() + dPosY);
    if (this->textureRect.has_value()) {
        sprite.setTextureRect(this->textureRect.value().getSfRect());
    }
    sprite.setScale(scaleX, scaleY);
    target.draw(sprite, states);
}


BOOST_CLASS_EXPORT_IMPLEMENT(Image)