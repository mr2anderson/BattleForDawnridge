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


#include "SelectWindow.hpp"


SelectWindow::SelectWindow(const std::string &soundName1, const std::string &soundName2, const std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> &data) : PopUpWindow(soundName1, soundName2) {
	this->data = data;
}
void SelectWindow::run(uint32_t windowW, uint32_t windowH) {
	this->playSound1();
	this->buttons.resize(this->data.size());
	for (uint32_t i = 0; i < this->data.size(); i = i + 1) {
		std::string pictureName = std::get<0>(this->data[i]);
		std::wstring message = std::get<1>(this->data[i]);
		bool clickable = std::get<2>(this->data[i]);
		GameEvent event = std::get<3>(this->data[i]);

		Button button(10, windowH - (64 + 10) * (i + 1), windowW - 20, 64, pictureName, message, 16);
		this->buttons[i] = std::make_tuple(button, clickable, event);
	}
}
void SelectWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		target.draw(std::get<Button>(this->buttons[i]), states);
	}
}
PopUpWindowEvent SelectWindow::click(uint32_t x, uint32_t y) const {
	for (const auto& t : buttons) {
		if (std::get<bool>(t) and std::get<Button>(t).click(x, y)) {
			PopUpWindowEvent popUpWindowEvent(true);
			popUpWindowEvent.gameEvent = std::get<GameEvent>(t);
			this->playSound2();
			return popUpWindowEvent;
		}
	}
	return PopUpWindowEvent(false);
}