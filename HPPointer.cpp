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



#include "HPPointer.hpp"


HPPointer::HPPointer() = default;
HPPointer::HPPointer(uint32_t current, uint32_t max, float xInPixels, float yInPixels, uint32_t sx, uint32_t sy) {
	this->current = current;
	this->max = max;
	this->xInPixels = xInPixels;
	this->yInPixels = yInPixels;
	this->sx = sx;
	this->sy = sy;
}
void HPPointer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint32_t red = 255 - 255 * this->current / this->max;
	uint32_t green = 255 * this->current / this->max;
	uint32_t blue = 0;

	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(64 * this->sx / 32, 64 * this->sy / 32));
	rect.setPosition(this->xInPixels + 64 * this->sx - rect.getSize().x - 1, this->yInPixels + 64 * this->sy / 6);
	rect.setFillColor(sf::Color(red, green, blue));
	target.draw(rect, states);
}