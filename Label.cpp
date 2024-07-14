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


#include <sstream>
#include "Label.hpp"
#include "Fonts.hpp"


Label::Label() = default;
Label::Label(int32_t x, int32_t y, uint32_t w, uint32_t h, std::wstring message, bool center) : RectangularUiElement(x, y, w, h) {
	this->initText();
	message = this->putNLs(message, w);
    if (center) {
        message = this->centerLines(message);
    }
    this->text.setString(message);
	this->center = center;
	this->rerenderNewPosition();
}
void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    this->RectangularUiElement::draw(target, states);
	target.draw(this->text, states);
}
void Label::setX(int32_t newX) {
	this->RectangularUiElement::setX(newX);
	this->rerenderNewPosition();
}
void Label::setY(int32_t newY) {
	this->RectangularUiElement::setY(newY);
	this->rerenderNewPosition();
}
void Label::initText() {
	this->text.setCharacterSize(13);
	this->text.setFillColor(sf::Color::White);
	this->text.setOutlineColor(sf::Color::Black);
	this->text.setOutlineThickness(1);
	this->text.setFont(*Fonts::get()->get("1"));
}
std::wstring Label::putNLs(const std::wstring& message, uint32_t w) {
	std::wstringstream ss(message);
	std::wstring prevMessage;
	std::wstring currentMessage;
	std::wstring word;
	while (std::getline(ss, word, L' ')) {
		word = word + L' ';
		prevMessage = currentMessage;
		currentMessage = currentMessage + word;
		this->text.setString(currentMessage);
		if (this->text.getLocalBounds().width > w - 10) {
			currentMessage = prevMessage + L'\n' + word;
			this->text.setString(currentMessage);
		}
	}
	return currentMessage;
}
std::wstring Label::centerLines(const std::wstring& message) {
	uint32_t maxWidth = this->getLongestLineWidth(message);
	return this->centerLines(message, maxWidth);
}
uint32_t Label::getLongestLineWidth(const std::wstring& message) {
	std::wstringstream ss = std::wstringstream(message);
	std::wstring word;
	uint32_t maxWidth = 0;
	while (std::getline(ss, word, L'\n')) {
		this->text.setString(word);
		maxWidth = std::max(maxWidth, (uint32_t)this->text.getLocalBounds().width);
	}
	return maxWidth;
}
std::wstring Label::centerLines(const std::wstring& message, uint32_t maxWidth) {
	std::wstringstream ss = std::wstringstream(message);
	std::wstring prevWord;
	std::wstring result;
	std::wstring word;
	while (std::getline(ss, word, L'\n')) {
		for (; ;) {
			prevWord = word;
			word = (L' ' + word + L' ');
			this->text.setString(word);
			if (this->text.getLocalBounds().width > maxWidth) {
				break;
			}
		}
		result = result + prevWord + L'\n';
	}
	return result;
}
void Label::rerenderNewPosition() {
	if (center) {
		this->text.setPosition(
			this->getX() + (int32_t)this->getW() / 2 - this->text.getLocalBounds().width / 2,
			this->getY() + 5);
	}
	else {
		this->text.setPosition(
			this->getX(),
			this->getY() + 5);
	}
}