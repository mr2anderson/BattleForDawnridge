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
HPBar::HPBar(uint32_t current, uint32_t max, uint32_t x, uint32_t y) {
	this->current = current;
	this->max = max;
	this->x = x;
	this->y = y;
}
void HPBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint32_t size = (64 - 4) * this->current / this->max;

	uint32_t posX = 64 * this->x + 2;
	uint32_t posY = 64 * this->y;
	
	uint32_t red = 255 - 255 * this->current / this->max;
	uint32_t green = 255 * this->current / this->max;
	uint32_t blue = 0;

	sf::RectangleShape rect1;
	rect1.setPosition(posX + 2, posY);
	rect1.setFillColor(sf::Color::Black);
	rect1.setSize(sf::Vector2f(64 - 4, 2));
	target.draw(rect1);

	sf::RectangleShape rect2;
	rect2.setPosition(sf::Vector2f(posX + 2, posY));
	rect2.setFillColor(sf::Color(red, green, blue));
	rect2.setSize(sf::Vector2f(size, 2));
	target.draw(rect2, states);

	sf::Text text;
	text.setFont(*FontStorage::get()->get("1"));
	text.setCharacterSize(10);
	text.setString(std::to_string(this->current));
	text.setFillColor(sf::Color(red, green, blue));
	text.setPosition(sf::Vector2f(posX + 64 - text.getLocalBounds().width, posY + rect1.getSize().y + 1));
	target.draw(text, states);
}
uint32_t HPBar::getCurrent() const {
	return this->current;
}
uint32_t HPBar::getMax() const {
	return this->max;
}
uint32_t HPBar::getX() const {
	return this->x;
}
uint32_t HPBar::getY() const {
	return this->y;
}
void HPBar::changeCurrent(uint32_t newCurrent) {
	this->current = std::min(this->max, newCurrent);
}
void HPBar::changeMax(uint32_t newMax) {
	this->max = newMax;
	this->current = std::min(this->current, this->max);
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