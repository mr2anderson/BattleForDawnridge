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


#include "RectangularUIElement.hpp"


RectangularUiElement::RectangularUiElement() = default;
RectangularUiElement::RectangularUiElement(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    this->x = x;
    this->y = y;

    this->w = w;
    this->h = h;
}
void RectangularUiElement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(w, h));
    rect.setPosition(sf::Vector2f(x, y));
    rect.setFillColor(UI_COLOR);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2);
    sf::View view = target.getView();
    target.setView(target.getDefaultView());
    target.draw(rect, states);
    target.setView(view);
}
uint32_t RectangularUiElement::getX() const {
    return this->x;
}
uint32_t RectangularUiElement::getY() const {
    return this->y;
}
uint32_t RectangularUiElement::getW() const {
    return this->w;
}
uint32_t RectangularUiElement::getH() const {
    return this->h;
}