/*
 *  Conquesta
 *  Copyright (C) 2023 mr2anderson
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



#include "Label.hpp"


Label::Label() = default;
Label::Label(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::string& message, uint32_t charSize) {
	this->rect.setSize(sf::Vector2f(w, h));
	this->rect.setPosition(sf::Vector2f(x, y));
	this->rect.setFillColor(UI_COLOR);
	this->rect.setOutlineColor(sf::Color::Black);
	this->rect.setOutlineThickness(2);

	this->text.setCharacterSize(charSize);
	this->text.setFillColor(sf::Color::White);
	this->text.setOutlineColor(sf::Color::Black);
	this->text.setOutlineThickness(1);
	this->text.setFont(*FontStorage::get()->get("1"));

	std::stringstream ss(message);
	std::string prevMessage;
	std::string currentMessage;
	std::string word;
	while (ss >> word) {
		word = word + " ";
		prevMessage = currentMessage;
		currentMessage = currentMessage + word;
		this->text.setString(currentMessage);
		if (this->text.getLocalBounds().width > w - 10) {
			currentMessage = prevMessage + "\n" + word;
			this->text.setString(currentMessage);
		}
	}

	this->text.setPosition(sf::Vector2f(this->rect.getPosition().x + w / 2 - this->text.getLocalBounds().width / 2, this->rect.getPosition().y + 5));
}
void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->rect, states);
	target.draw(this->text, states);
}
uint32_t Label::getX() const {
	return this->rect.getPosition().x;
}
uint32_t Label::getY() const {
	return this->rect.getPosition().y;
}
uint32_t Label::getW() const {
	return this->rect.getSize().x;
}
uint32_t Label::getH() const {
	return this->rect.getSize().y;
}