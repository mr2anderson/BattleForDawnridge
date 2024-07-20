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


#include "SpellPreview.hpp"


SpellPreview::SpellPreview(uint32_t radius, sf::Color color, uint32_t centerX, uint32_t centerY) {
	this->radius = radius;
	this->color = color;
	this->centerX = centerX;
	this->centerY = centerY;
}
void SpellPreview::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint32_t xMin;
	if (this->centerX >= this->radius) {
		xMin = this->centerX - this->radius;
	}
	else {
		xMin = 0;
	}

	uint32_t yMin;
	if (this->centerY >= this->radius) {
		yMin = this->centerY - this->radius;
	}
	else {
		yMin = 0;
	}

	uint32_t xMax = this->centerX + this->radius;
	uint32_t yMax = this->centerY + this->radius;

	for (uint32_t x = xMin; x <= xMax; x = x + 1) {
		for (uint32_t y = yMin; y <= yMax; y = y + 1) {
			sf::RectangleShape rect;

			rect.setPosition(64 * x + 1, 64 * y + 1);
			rect.setSize(sf::Vector2f(62, 62));
			rect.setFillColor(this->color);
			rect.setOutlineColor(sf::Color::Black);
			rect.setOutlineThickness(1);

			target.draw(rect, states);
		}
	}
}