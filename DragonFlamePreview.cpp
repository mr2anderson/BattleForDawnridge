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


#include "DragonFlamePreview.hpp"


DragonFlamePreview::DragonFlamePreview() = default;
void DragonFlamePreview::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (uint32_t i = 0; i < this->rects.size(); i = i + 1) {
		target.draw(this->rects.at(i));
	}
}
void DragonFlamePreview::addCellUnderTheFlame(uint32_t x, uint32_t y) {
	sf::RectangleShape rect;
	rect.setPosition(64 * x + 1, 64 * y + 1);
	rect.setFillColor(this->getPreviewCellColor());
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(1);
	rect.setSize(sf::Vector2f(64 - 2, 64 - 2));

	this->rects.push_back(rect);
}