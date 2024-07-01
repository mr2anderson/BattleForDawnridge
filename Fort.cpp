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
#include "Fort.hpp"


Fort::Fort(uint32_t x, uint32_t y, const Player* playerPtr) : Building(x, y, 100000, true, playerPtr) {}
std::string Fort::getTextureName() const {
	return "fort";
}
GameObjectResponse Fort::getGameObjectResponse(const Player& player, uint32_t windowW, uint32_t windowH) {
	if (this->belongTo(&player)) {
		GameObjectResponse response;
		std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> data;
		data.emplace_back("exit", L"Покинуть", true, GameEvent());
		data.emplace_back("fort", L"Замок — сердце города. Разгром замка приведет к поражению. Защищайте его любой ценой.", false, GameEvent());
		SelectWindow* window = new SelectWindow("hooray", "click", data);
		response.popUpWindows.push(window);
		return response;
	}

	GameObjectResponse response;
	response.gameEvent = GameEvent();
	response.gameEvent.value().tryToAttack = this;
	return response;
}