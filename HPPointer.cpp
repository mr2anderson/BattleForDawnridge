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
HPPointer::HPPointer(float xInPixels, float yInPixels, uint32_t sx, uint32_t sy, uint8_t orientation) {
	this->xInPixels = xInPixels;
	this->yInPixels = yInPixels;
	this->sx = sx;
	this->sy = sy;
	this->orientation = orientation;
}
void HPPointer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint32_t red = 255 - 255 * this->current / this->max;
	uint32_t green = 255 * this->current / this->max;
	uint32_t blue = 0;

	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(4, 4));
	rect.setPosition(this->xInPixels + (this->orientation == ORIENTATION::LEFT_UP) * 15 + (this->orientation == ORIENTATION::RIGHT_UP) * (64 * this->sx - rect.getSize().x - 1), this->yInPixels + (this->orientation == ORIENTATION::RIGHT_UP) * 15);
	rect.setFillColor(sf::Color(red, green, blue));
	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color::Black);
	target.draw(rect, states);
}
void HPPointer::setCurrent(uint32_t current) {
	this->current = current;
}
void HPPointer::setMax(uint32_t max) {
	this->max = max;
}