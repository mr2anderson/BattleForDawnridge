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


#include "MessageWindow.hpp"


MessageWindow::MessageWindow(uint32_t windowW, uint32_t windowH, const std::wstring& message) : PopUpWindow(windowW, windowH) {
	uint32_t w = 200;
	uint32_t h = 100;
	uint32_t buttonW = 50;
	uint32_t buttonH = 25;
	this->label = Label((this->getWindowW() - w) / 2, (this->getWindowH() - h) / 2, w, h, message, 13);
	this->button = Button((this->getWindowW() - w) / 2 + (w - buttonW) / 2, (this->getWindowH() - h) / 2 + h - buttonH - 5, buttonW, buttonH, L"OK", 18);
}
void MessageWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->label, states);
	target.draw(this->button, states);
}
PopUpWindowEvent MessageWindow::click(uint32_t x, uint32_t y) const {
	PopUpWindowEvent event(this->button.click(x, y));
	if (event.close) {
		event.gameEvent.playSound = "click";
	}
	return event;
}