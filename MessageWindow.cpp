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


#include "MessageWindow.hpp"


MessageWindow::MessageWindow(uint32_t windowW, uint32_t windowH, const std::string& message) : PopUpWindow(windowW, windowH) {
	this->mainRect.setSize(sf::Vector2f(mainRectW, mainRectH));
	this->mainRect.setPosition(sf::Vector2f((this->getWindowW() - mainRectW) / 2, (this->getWindowH() - mainRectH) / 2));
	this->mainRect.setFillColor(UI_COLOR);
	this->mainRect.setOutlineColor(sf::Color::Black);
	this->mainRect.setOutlineThickness(2);

	this->button.setSize(sf::Vector2f(buttonW, buttonH));
	this->button.setPosition(this->mainRect.getPosition().x + mainRectW / 2 - buttonW / 2, this->mainRect.getPosition().y + mainRectH - buttonH - 10);
	this->button.setFillColor(UI_COLOR);
	this->button.setOutlineColor(sf::Color::Black);
	this->button.setOutlineThickness(1);

	this->buttonText.setCharacterSize(14);
	this->buttonText.setFillColor(sf::Color::White);
	this->buttonText.setOutlineColor(sf::Color::Black);
	this->buttonText.setOutlineThickness(1);
	this->buttonText.setFont(*FontStorage::get()->get("1"));
	this->buttonText.setString("OK");
	this->buttonText.setPosition(this->button.getPosition().x + buttonW / 2 - this->buttonText.getLocalBounds().width / 2, this->button.getPosition().y + buttonH / 2 - this->buttonText.getLocalBounds().height / 2);

	this->messageText.setCharacterSize(14);
	this->messageText.setFillColor(sf::Color::White);
	this->messageText.setOutlineColor(sf::Color::Black);
	this->messageText.setOutlineThickness(1);
	this->messageText.setFont(*FontStorage::get()->get("1"));

	std::stringstream ss(message);
	std::string prevMessage;
	std::string currentMessage;
	std::string word;
	while (ss >> word) {
		word = word + " ";
		prevMessage = currentMessage;
		currentMessage = currentMessage + word;
		this->messageText.setString(currentMessage);
		if (this->messageText.getLocalBounds().width > mainRectW - 20) {
			currentMessage = prevMessage + "\n" + word;
			this->messageText.setString(currentMessage);
		}
	}

	this->messageText.setPosition(sf::Vector2f(this->mainRect.getPosition().x + mainRectW / 2 - this->messageText.getLocalBounds().width / 2, this->mainRect.getPosition().y + 10));
	
}
void MessageWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->mainRect, states);
	target.draw(this->messageText, states);

	target.draw(this->button, states);
	target.draw(this->buttonText, states);
}
PopUpWindowEvent MessageWindow::click(uint32_t x, uint32_t y) const {
	if (x >= this->button.getPosition().x and
		y >= this->button.getPosition().y and
		x < this->button.getPosition().x + this->buttonW and
		y < this->button.getPosition().y + this->buttonH) {
		return PopUpWindowEvent(true);
	}
	return PopUpWindowEvent(false);
}