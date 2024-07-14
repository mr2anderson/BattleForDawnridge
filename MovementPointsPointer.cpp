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


#include "MovementPointsPointer.hpp"
#include "Fonts.hpp"


const sf::Color MovementPointsPointer::BLUE = sf::Color(0, 0, 220);
const sf::Color MovementPointsPointer::GREEN = sf::Color(0, 140, 0);
const sf::Color MovementPointsPointer::PURPLE = sf::Color(112, 0, 195);


MovementPointsPointer::MovementPointsPointer(float xInPixels, float yInPixels, uint32_t value) {
    this->text.setFont(*Fonts::get()->get("1"));
    this->text.setCharacterSize(10);
    this->text.setString(std::to_string(value));
    this->text.setPosition(xInPixels + 64 - this->text.getGlobalBounds().width, yInPixels);
}
void MovementPointsPointer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(this->text, states);
}
void MovementPointsPointer::setTypeBlue() {
    this->text.setFillColor(BLUE);
}
void MovementPointsPointer::setTypeGreen() {
    this->text.setFillColor(GREEN);
}
void MovementPointsPointer::setTypePurple() {
    this->text.setFillColor(PURPLE);
}