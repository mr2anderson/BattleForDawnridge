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



#include "WarriorHPPointer.hpp"
#include "Textures.hpp"


WarriorHPPointer::WarriorHPPointer() = default;
WarriorHPPointer::WarriorHPPointer(float xInPixels, float yInPixels, uint32_t sx, uint32_t sy, uint32_t currentHP, uint32_t maxHP) {
	this->xInPixels = xInPixels;
	this->yInPixels = yInPixels;

	this->sx = sx;
	this->sy = sy;

    this->current = currentHP;
    this->max = maxHP;
}
void WarriorHPPointer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint32_t red = 255 - 255 * this->current / this->max;
	uint32_t green = 255 * this->current / this->max;
	uint32_t blue = 0;

    sf::Sprite sprite;
    sprite.setTexture(*Textures::get()->get("heart_icon"));
    sprite.setPosition(this->xInPixels + 64 * this->sx - sprite.getGlobalBounds().width * 2, this->yInPixels + sprite.getGlobalBounds().height);
    sprite.setColor(sf::Color(red, green, blue));
    target.draw(sprite, states);
}