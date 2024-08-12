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


#include "HPFlyingE.hpp"
#include "Fonts.hpp"


static const uint32_t CHAR_SIZE = 18;


HPFlyingE::HPFlyingE() = default;
HPFlyingE::HPFlyingE(uint32_t delta, bool plus, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) : FlyingE(64 * x + 64 * sx / 2 - CHAR_SIZE * std::to_string(delta).size() / 2, 64 * y + 64 * sy / 2 - CHAR_SIZE * std::to_string(delta).size() / 2)  {
    this->delta = delta;
    this->plus = plus;
}
std::unique_ptr<sf::Drawable> HPFlyingE::getDrawable(sf::Vector2f position, sf::Color color) const {
    sf::Text text;
    text.setFont(*Fonts::get()->get("1"));
    text.setString(std::to_string(delta));
    if (plus) {
        text.setFillColor(sf::Color(0, 255, 0, color.a));
    }
    else {
        text.setFillColor(sf::Color(255, 0, 0, color.a));
    }
    text.setCharacterSize(CHAR_SIZE);
    text.setPosition(position);

    return std::make_unique<sf::Text>(text);
}


BOOST_CLASS_EXPORT_IMPLEMENT(HPFlyingE)