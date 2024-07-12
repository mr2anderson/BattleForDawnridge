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


#include <array>
#include "PlayerPointer.hpp"
#include "Textures.hpp"


PlayerPointer::PlayerPointer(uint32_t side, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->side = side;
	this->x = x;
	this->y = y;
	this->sx = sx;
	this->sy = sy;
}
void PlayerPointer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	std::array<std::string, 6> colors = {
		"yellow",
		"purple",
		"green",
		"blue",
		"black",
		"grey"
	};
	std::string color = colors.at(this->side - 1);
	sf::Sprite pointer;
	pointer.setTexture(*Textures::get()->get(color));
	float scale = std::max(0.5f, 64 * (float)this->sy / (float)pointer.getTexture()->getSize().y / 12);
	pointer.setScale(scale, scale);
	pointer.setPosition(64 * (this->x + this->sx) - pointer.getLocalBounds().width, 64 * this->y);
	target.draw(pointer, states);
}