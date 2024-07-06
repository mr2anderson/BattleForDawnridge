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


#include "SimpleWindow.hpp"


SimpleWindow::SimpleWindow(const std::string &soundName1, const std::string &soundName2, const std::wstring& message, const std::wstring &buttonText, uint32_t w, uint32_t h) :
	PopUpElement() {
    this->w = w;
    this->h = h;
	this->soundName1 = soundName1;
	this->soundName2 = soundName2;
	this->message = message;
    this->buttonText = buttonText;
}
Event SimpleWindow::run(uint32_t windowW, uint32_t windowH) {
	uint32_t buttonW = 75;
	uint32_t buttonH = 30;

	this->label = Label((windowW - this->w) / 2, (windowH - this->h) / 2, this->w, this->h, std::nullopt, message);
	this->button = Button((windowW - this->w) / 2 + (this->w - buttonW) / 2, (windowH - this->h) / 2 + h - buttonH - 15, buttonW, buttonH, std::nullopt, buttonText);

	Event soundEvent;
	soundEvent.playSound.push_back(this->soundName1);

	return this->PopUpElement::run(windowW, windowH) + soundEvent;
}
void SimpleWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(label, states);
	target.draw(button, states);
}
Event SimpleWindow::click() {
	if (this->button.click()) {
		this->finish();
		Event event;
		event.playSound.push_back(this->soundName2);
		return event;
	}
	return Event();
}
void SimpleWindow::update() {

}