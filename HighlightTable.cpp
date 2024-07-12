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
	const Unit* u = e.getUnit();
	for (uint32_t dx = 0; dx < e.getSX(); dx = dx + 1) {
		for (uint32_t dy = 0; dy < e.getSY(); dy = dy + 1) {
			std::tuple<uint32_t, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t> key = std::make_tuple(e.getX() + dx, e.getY() + dy, e.getColor().r, e.getColor().g, e.getColor().b, e.getColor().a);
			std::map<const Unit*, bool> byUnits = this->data[key];
			if (byUnits.find(u) == byUnits.end()) {
				byUnits[u] = true;
			}
			else {
				byUnits.erase(u);
			}
			this->data[key] = byUnits;
		}
	}
}
std::vector<sf::RectangleShape> HighlightTable::getRects() const {
	std::vector<sf::RectangleShape> rects;
	rects.reserve(this->data.size());
	for (const auto& p : this->data) {
		if (!p.second.empty()) {
			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f(64, 64));
			rect.setPosition(64 * std::get<0>(p.first), 64 * std::get<1>(p.first));
			rect.setFillColor(sf::Color(std::get<2>(p.first), std::get<3>(p.first), std::get<4>(p.first), std::get<5>(p.first)));
			rect.setOutlineThickness(1);
			rect.setOutlineColor(sf::Color::Black);
			rects.push_back(rect);
		}
	}
	return rects;
}