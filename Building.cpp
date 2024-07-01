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


#include "Building.hpp"


Building::Building(uint32_t x, uint32_t y, uint32_t maxHp, const Player* playerPtr) : Building(x, y, maxHp, false, playerPtr) {}
Building::Building(uint32_t x, uint32_t y, uint32_t maxHp, bool full, const Player* playerPtr) : Unit(x, y, full * maxHp + (!full) * 1, maxHp, playerPtr) {}
GameObjectResponse Building::newMove(const Player& currentPlayer, uint32_t windowW, uint32_t windowH) {
	GameObjectResponse response = this->Unit::newMove(currentPlayer, windowW, windowH);
	if (!this->belongTo(&currentPlayer)) {
		return response;
	}
	bool works1 = this->works();
	this->addHp(REGENERATION_SPEED);
	bool works2 = this->works();
	if (works2 and !works1 and this->isHpSensitive()) {
		MessageWindow* window = new MessageWindow("hooray", "click", L"Постройка достроена!\nБлагодаря усилиям рабочих, " + this->getName() + L" теперь может работать.");
		response.popUpWindows.push(window);
	}
	return response;
}
bool Building::works() const {
	return (100 * this->getHP() / this->getMaxHP() >= MINIMAL_PERCENT);
}