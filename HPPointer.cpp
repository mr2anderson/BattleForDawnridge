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
void HPPointer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint32_t red = 127 - 127 * this->current / this->max;
	uint32_t green = 127 * this->current / this->max;
	uint32_t blue = 0;

	sf::Text text;
	text.setFillColor(sf::Color(red, green, blue));
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setFont(*Fonts::get()->get("1"));
	text.setCharacterSize(8 + 2 * this->sx);
	text.setString(this->getCurrentHpFormatted());
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
std::string HPPointer::getCurrentHpFormatted() const {
	double d = (double)this->current / 1000;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << d;

	std::string s = ss.str();
	if (s.back() == '0') {
		s.pop_back();
		s.pop_back();
	}

	return s + "k";
}