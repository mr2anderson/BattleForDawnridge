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


#include "MessageW.hpp"


MessageW::MessageW(const std::string &soundName1, const std::string &soundName2, const std::wstring& message) : 
	PopUpW() {
	this->soundName1 = soundName1;
	this->soundName2 = soundName2;
	this->message = message;
}
Events MessageW::run(uint32_t windowW, uint32_t windowH) {
	uint32_t w = 400;
	uint32_t h = 225;
	uint32_t buttonW = 50;
	uint32_t buttonH = 28;

	this->label = Label((windowW - w) / 2, (windowH - h) / 2, w, h, std::nullopt, message);
	this->button = Button((windowW - w) / 2 + (w - buttonW) / 2, (windowH - h) / 2 + h - buttonH - 10, buttonW, buttonH, std::nullopt, L"OK");

	UIEvent soundEvent;
	soundEvent.playSound.push_back(this->soundName1);

	return Events(this->getOnStartGEvent(), soundEvent);
}
void MessageW::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(label, states);
	target.draw(button, states);
}
Events MessageW::click(uint32_t x, uint32_t y) const {
	if (this->button.click(x, y)) {
		UIEvent event;
		event.playSound.push_back(this->soundName2);
		event.closePopUpWindows = 1;
		return Events(GEvent(), event);
	}
	return Events();
}