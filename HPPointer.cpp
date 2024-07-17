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



#include <sstream>
#include <iomanip>
#include "HPPointer.hpp"
#include "Fonts.hpp"


HPPointer::HPPointer() = default;
HPPointer::HPPointer(float xInPixels, float yInPixels, uint32_t sx, uint32_t sy, uint8_t orientation) {
	this->xInPixels = xInPixels;
	this->yInPixels = yInPixels;
	this->sx = sx;
	this->sy = sy;
	this->orientation = orientation;
}
static std::string FORMAT_FLOAT(double a) {
	std::stringstream ss;
	ss << std::setprecision(8) << std::noshowpoint << a;
	return ss.str();
}
void HPPointer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint32_t red = 255 - 255 * this->current / this->max;
	uint32_t green = 255 * this->current / this->max;
	uint32_t blue = 0;

	std::string str;
	str += FORMAT_FLOAT((float)this->current / 1000);
	str += "k";

	sf::Text text;
	text.setFillColor(sf::Color(red, green, blue));
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setFont(*Fonts::get()->get("1"));
	text.setCharacterSize(6 + 2 * this->sx);
	text.setString(str);
	text.setPosition(
		this->xInPixels + 64 * this->sx / 2 - text.getGlobalBounds().width / 2, 
		this->yInPixels + 5 + (this->orientation == ORIENTATION::DOWN) * (64 * this->sy - 2 * 5 - text.getGlobalBounds().height));

	target.draw(text, states);
}
void HPPointer::setCurrent(uint32_t current) {
	this->current = current;
}
void HPPointer::setMax(uint32_t max) {
	this->max = max;
}