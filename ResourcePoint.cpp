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


ResourcePoint::ResourcePoint(uint32_t x, uint32_t y, uint32_t size) : GameObject(x, y, size, size) {}
GameObjectResponse ResourcePoint::getGameObjectResponse(const Player& player, uint32_t windowW, uint32_t windowH) {
	GameObjectResponse response;
	std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> data;
	data.emplace_back("exit", L"Вернуться", true, GameEvent());
	data.emplace_back(this->getIconName(), this->getDescription(), false, GameEvent());
	response.popUpWindow = new SelectWindow(this->getClickSoundName(), "click", data);
	return response;
}