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



#include "Label.hpp"


Label::Label() = default;
Label::Label(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::wstring &message, uint32_t charSize) : Label(x, y, w, h, "", message, charSize) {}
Label::Label(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::string& picture, const std::wstring& message, uint32_t charSize) {
	this->rect.setSize(sf::Vector2f(w, h));
	this->rect.setPosition(sf::Vector2f(x, y));
	this->rect.setFillColor(UI_COLOR);
	this->rect.setOutlineColor(sf::Color::Black);
	this->rect.setOutlineThickness(2);

	if (picture != "") {
		this->sprite.setPosition(this->rect.getPosition());
		this->sprite.setTexture(*TextureStorage::get()->get(picture));
	}

	this->text.setCharacterSize(charSize);
	this->text.setFillColor(sf::Color::White);
	this->text.setOutlineColor(sf::Color::Black);
	this->text.setOutlineThickness(1);
	this->text.setFont(*FontStorage::get()->get("1"));

	std::wstringstream ss(message);
	std::wstring prevMessage;
	std::wstring currentMessage;
	std::wstring word;
	while (std::getline(ss, word, L' ')) {
		word = word + L" ";
		prevMessage = currentMessage;
		currentMessage = currentMessage + word;
		this->text.setString(currentMessage);
		if (this->text.getLocalBounds().width > w - 10 - this->sprite.getLocalBounds().width * (picture != "")) {
			currentMessage = prevMessage + L"\n" + word;
			this->text.setString(currentMessage);
		}
	}

	this->text.setPosition(0, this->rect.getPosition().y + 5);
	if (picture == "") {
		this->text.setPosition(sf::Vector2f(sf::Vector2f(this->rect.getPosition().x + w / 2 - this->text.getLocalBounds().width / 2, this->text.getPosition().y)));
	}
	else {
		this->text.setPosition(sf::Vector2f(this->sprite.getPosition().x + this->sprite.getLocalBounds().width + 5, this->text.getPosition().y));
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