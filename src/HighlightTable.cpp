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
#include "ColorBlender.hpp"


HighlightTable::HighlightTable() = default;
void HighlightTable::clear() {
	this->data.clear();
}
void HighlightTable::mark(SetHighlightEvent e) {
    std::tuple<uint32_t, uint32_t> key = std::make_tuple(e.getX(), e.getY());
    this->data[key].insert(SerializableColor(e.getColor()));
}


static std::vector<sf::Color> TO_SF_VECTOR(const std::set<SerializableColor> &set) {
    std::vector<sf::Color> v;
    for (const auto &c : set) {
        v.push_back(c.getSfColor());
    }
    return v;
}


std::vector<sf::RectangleShape> HighlightTable::getRects() const {
	std::vector<sf::RectangleShape> rects;
	rects.reserve(this->data.size());
	for (const auto& p : this->data) {
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(64, 64));
        rect.setPosition(64 * std::get<0>(p.first), 64 * std::get<1>(p.first));
        rect.setFillColor(ColorBlender::get()->blend(TO_SF_VECTOR(p.second)));
        rect.setOutlineThickness(1);
        rect.setOutlineColor(sf::Color::Black);
        rects.push_back(rect);
	}
	return rects;
}