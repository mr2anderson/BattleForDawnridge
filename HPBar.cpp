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



#include "HPBar.hpp"


HPBar::HPBar() = default;
HPBar::HPBar(uint32_t current, uint32_t max, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->current = current;
	this->max = max;
	this->x = x;
	this->y = y;
	this->sx = sx;
	this->sy = sy;
}
void HPBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint32_t size = (64 / 4 * this->sy - 64 * this->sy / 8) * this->current / this->max;

	uint32_t posX = 64 * this->x + 4;
	uint32_t posY = 64 * this->y + 4;
	
	uint32_t red = 255 - 255 * this->current / this->max;
	uint32_t green = 255 * this->current / this->max;
	uint32_t blue = 0;

	sf::RectangleShape rect1;
	rect1.setPosition(posX, posY);
	rect1.setFillColor(sf::Color::Black);
	rect1.setOutlineColor(sf::Color::Black);
	rect1.setOutlineThickness(1);
	rect1.setSize(sf::Vector2f(2, 64 / 4 * this->sy - 64 * this->sy / 8));
	target.draw(rect1);

	sf::RectangleShape rect2;
	rect2.setPosition(sf::Vector2f(posX, posY) + sf::Vector2f(0, 64 / 4 * this->sy - 64 * this->sy / 8 - size));
	rect2.setFillColor(sf::Color(red, green, blue));
	rect2.setSize(sf::Vector2f(2, size));
	target.draw(rect2, states);
}