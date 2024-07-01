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


SelectWindow::SelectWindow(uint32_t windowW, uint32_t windowH, const std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> &data) : PopUpWindow(windowW, windowH) {
	for (const auto& t : data) {
		Button button(10, this->getWindowH() - (64 + 10) * (this->data.size() + 1), this->getWindowW() - 20, 64, std::get<std::string>(t), std::get<std::wstring>(t), 16);
		this->data.emplace_back(button, std::get<bool>(t), std::get<GameEvent>(t));
	}
}
void SelectWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (uint32_t i = 0; i < this->data.size(); i = i + 1) {
		target.draw(std::get<Button>(this->data[i]), states);
	}
}
PopUpWindowEvent SelectWindow::click(uint32_t x, uint32_t y) const {
	for (const auto& t : data) {
		if (std::get<bool>(t) and std::get<Button>(t).click(x, y)) {
			PopUpWindowEvent popUpWindowEvent(true);
			popUpWindowEvent.gameEvent = std::get<GameEvent>(t);
			return popUpWindowEvent;
		}
	}
	return PopUpWindowEvent(false);
}