/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "HPBar.hpp"


HPBar::HPBar() = default;
HPBar::HPBar(uint32_t current, uint32_t max, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->current = current;
	this->max = max;
	this->x = x;
	this->y = y;
	this->sx = sx;
	this->sy = sy;
}
void HPBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint32_t size = (32 / 4 * this->sy - 8) * this->current / this->max;

	uint32_t posX = 32 * this->x + 4;
	uint32_t posY = 32 * this->y + 4;
	
	uint32_t red = 255 - 255 * this->current / this->max;
	uint32_t green = 255 * this->current / this->max;
	uint32_t blue = 0;

	sf::RectangleShape rect1;
	rect1.setPosition(posX, posY);
	rect1.setFillColor(sf::Color::Black);
	rect1.setOutlineColor(sf::Color::Black);
	rect1.setOutlineThickness(1);
	rect1.setSize(sf::Vector2f(2, 32 / 4 * this->sy - 8));
	target.draw(rect1);

	sf::RectangleShape rect2;
	rect2.setPosition(sf::Vector2f(posX, posY) + sf::Vector2f(0, 32 / 4 * this->sy - 8 - size));
	rect2.setFillColor(sf::Color(red, green, blue));
	rect2.setSize(sf::Vector2f(2, size));
	target.draw(rect2, states);
}
uint32_t HPBar::getCurrent() const {
	return this->current;
}
uint32_t HPBar::getMax() const {
	return this->max;
}
void HPBar::changeCurrent(uint32_t newCurrent) {
	this->current = std::min(this->max, newCurrent);
}
void HPBar::changeMax(uint32_t newMax) {
	this->max = newMax;
	this->current = std::min(this->current, this->max);
}
void HPBar::setMax() {
	this->current = this->max;
}
HPBar operator+(HPBar a, uint32_t b) {
	a.current = std::min(a.current + b, a.max);
	return a;
}
HPBar operator-(HPBar a, uint32_t b) {
	if (b >= a.current) {
		a.current = 0;
	}
	else {
		a.current = a.current - b;
	}
	return a;
}
std::wstring HPBar::getReadableInfo() const {
	return L"Защита: " + std::to_wstring(this->getCurrent()) + L" / " + std::to_wstring(this->getMax());
}
uint32_t HPBar::getX() const {
	return this->x;
}
uint32_t HPBar::getY() const {
	return this->y;
}
uint32_t HPBar::getSX() const {
	return this->sx;
}
uint32_t HPBar::getSY() const {
	return this->sy;
}