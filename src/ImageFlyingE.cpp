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


#include "ImageFlyingE.hpp"
#include "Textures.hpp"


ImageFlyingE::ImageFlyingE() = default;
ImageFlyingE::ImageFlyingE(const std::string &textureName, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) : FlyingE(64 * x + sx * 64 / 2 - Textures::get()->get(textureName)->getSize().x / 2, 64 * y + sy * 64 / 2 - Textures::get()->get(textureName)->getSize().y / 2)  {
    this->textureName = textureName;
}
std::unique_ptr<sf::Drawable> ImageFlyingE::getDrawable(sf::Vector2f position, sf::Color color) const {
    sf::Sprite sprite;
    sprite.setPosition(position);
    sprite.setColor(color);
    sprite.setTexture(*Textures::get()->get(this->textureName));
    sprite.setScale(32 / sprite.getLocalBounds().width, 32 / sprite.getLocalBounds().height);

    return std::make_unique<sf::Sprite>(sprite);
}


BOOST_CLASS_EXPORT_IMPLEMENT(ImageFlyingE)