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


HPFlyingE::HPFlyingE(uint32_t current, uint32_t max, bool green, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
    sf::Text text;
    text.setFont(*Fonts::get()->get("1"));
    text.setString(std::to_string(current) + " / " + std::to_string(max));
    if (green) {
        text.setFillColor(sf::Color::Green);
    }
    else {
        text.setFillColor(sf::Color::Red);
    }
    text.setCharacterSize(6 + 4 * std::min(sx, sy));
    text.setPosition(64 * x + 64 * sx / 2 - text.getGlobalBounds().width / 2, 64 * y + 64 * sy / 2 - text.getGlobalBounds().height / 2);
    this->set(text);
}
void HPFlyingE::setTransparentColor(float dt) {
    sf::Color old = this->getTPtr()->getFillColor();
    old.a = this->getTransparencyLevel(dt).a;
    this->getTPtr()->setFillColor(old);
}