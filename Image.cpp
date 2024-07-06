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
Image::Image(uint32_t x, uint32_t y, const std::string &imageName) :
        RectangularUiElement(x, y, Textures::get()->get(imageName)->getSize().x, Textures::get()->get(imageName)->getSize().y) {
    this->sprite.setTexture(*Textures::get()->get(imageName));
    this->sprite.setPosition(x, y);
}
Image::Image(uint32_t x, uint32_t y, uint32_t h, const std::string &imageName) :
        RectangularUiElement(x, y, (float)Textures::get()->get(imageName)->getSize().x * (float)h / (float)Textures::get()->get(imageName)->getSize().y, h) {
    this->sprite.setTexture(*Textures::get()->get(imageName));
    this->sprite.setPosition(x, y);
    float scale = (float)h / (float)Textures::get()->get(imageName)->getSize().y;
    this->sprite.setScale(scale, scale);
}
void Image::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    this->RectangularUiElement::draw(target, states);
    sf::View v = target.getView();
    target.setView(target.getDefaultView());
    target.draw(this->sprite, states);
    target.setView(v);
}