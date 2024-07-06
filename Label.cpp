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



#include "Label.hpp"


Label::Label() = default;
Label::Label(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::optional<std::string>& picture, std::wstring message) {
	this->setRect(x, y, w, h);
	if (picture.has_value()) {
		this->setPicture(w, h, picture.value());
	}
	this->initText();
	message = this->putNLs(message, w, picture.has_value());
	if (picture.has_value()) {
		this->text.setString(message);
		this->text.setPosition(
			x + this->sprite.getLocalBounds().width * this->sprite.getScale().x + 5, 
			y + 5);
	}
	else {
		message = this->centerLines(message);
		this->text.setString(message);
		this->text.setPosition(
			x + w / 2 - this->text.getLocalBounds().width / 2,
			y + 5);
	}
}
void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::View old = target.getView();
	target.setView(target.getDefaultView());
	target.draw(this->rect, states);
	target.draw(this->sprite);
	target.draw(this->text, states);
	target.setView(old);
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
void Label::setRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
	this->rect.setSize(sf::Vector2f(w, h));
	this->rect.setPosition(sf::Vector2f(x, y));
	this->rect.setFillColor(UI_COLOR);
	this->rect.setOutlineColor(sf::Color::Black);
	this->rect.setOutlineThickness(2);
}
void Label::setPicture(uint32_t w, uint32_t h, const std::string& picture) {
	this->sprite.setPosition(this->rect.getPosition());
	this->sprite.setTexture(*Textures::get()->get(picture));
	float scaleX = w / (float)this->sprite.getTexture()->getSize().x;
	float scaleY = h / (float)this->sprite.getTexture()->getSize().y;
	float scale = std::min(scaleX, scaleY);
	this->sprite.setScale(scale, scale);
}
void Label::initText() {
	this->text.setCharacterSize(14);
	this->text.setFillColor(sf::Color::White);
	this->text.setOutlineColor(sf::Color::Black);
	this->text.setOutlineThickness(1);
	this->text.setFont(*Fonts::get()->get("1"));
}
std::wstring Label::putNLs(const std::wstring& message, uint32_t w, bool picture) {
	std::wstringstream ss(message);
	std::wstring prevMessage;
	std::wstring currentMessage;
	std::wstring word;
	while (std::getline(ss, word, L' ')) {
		word = word + L' ';
		prevMessage = currentMessage;
		currentMessage = currentMessage + word;
		this->text.setString(currentMessage);
		if (this->text.getLocalBounds().width > w - 10 - this->sprite.getLocalBounds().width * picture) {
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