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


#include "ResourcePoint.hpp"


ResourcePoint::ResourcePoint() = default;
ResourcePoint::ResourcePoint(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t size) : GO(x, y, sx, sy, size, size) {}
GOR ResourcePoint::newMove(const Player& player) {
	return GOR();
}
GOR ResourcePoint::getGameObjectResponse(const Player& player) {
	if (!this->exist()) {
		return GOR();
	}
	return this->getSelectionWindow();
}
GOR ResourcePoint::getSelectionWindow() {
	GOR response;

	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", L"Вернуться", true, true, GEvent());
	components.emplace_back(this->getTextureName(), this->getDescription(), false, false, GEvent());

	SelectionW* window = new SelectionW(this->getClickSoundName(), "click", components);
	response.elements.push(window);

	return response;
}