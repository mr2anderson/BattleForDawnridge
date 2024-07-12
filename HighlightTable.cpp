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


#include "HighlightTable.hpp"


HighlightTable::HighlightTable() = default;
void HighlightTable::clear() {
	this->data.clear();
}
void HighlightTable::mark(ChangeHighlightEvent e) {
	if (this->data.find(e) == this->data.end()) {
		this->data[e] = true;
	}
	else {
		this->data.erase(e);
	}
}
std::vector<sf::RectangleShape> HighlightTable::getRects() const {
	std::vector<sf::RectangleShape> rects;
	rects.reserve(this->data.size());
	for (const auto& e : this->data) {
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(64 * e.first.getSX() - 4, 64 * e.first.getSY() - 4));
		rect.setPosition(64 * e.first.getX() + 2, 64 * e.first.getY() + 2);
		rect.setFillColor(e.first.getColor());
		rect.setOutlineThickness(1);
		rect.setOutlineColor(sf::Color::Black);
		rects.push_back(rect);
	}
	return rects;
}