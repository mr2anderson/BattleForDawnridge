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


#include "Button.hpp"



Button::Button() = default;
Button::Button(std::shared_ptr<RectangularUiElement> element, const Events &onClick) {
    this->element = element;
    this->onClick = onClick;
}
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(*this->element, states);
}
Events Button::click(uint32_t mouseX, uint32_t mouseY) const {
	if (mouseX >= this->element->getX() and
        mouseY >= this->element->getY() and
        mouseX < this->element->getX() + this->element->getW() and
        mouseY < this->element->getY() + this->element->getH()) {
        return this->onClick;
    }
    return Events();
}
void Button::setX(int32_t newX) {
    this->element->setX(newX);
}
void Button::setY(int32_t newY) {
    this->element->setY(newY);
}
int32_t Button::getX() const {
    return this->element->getX();
}
int32_t Button::getY() const {
    return this->element->getY();
}
uint32_t Button::getW() const {
    return this->element->getW();
}
uint32_t Button::getH() const {
    return this->element->getH();
}