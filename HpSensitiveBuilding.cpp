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


#include "HpSensitiveBuilding.hpp"


HpSensitiveBuilding::HpSensitiveBuilding() = default;
HpSensitiveBuilding::HpSensitiveBuilding(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, bool full, const Player* playerPtr) : Building(x, y, sx, sy, maxHp, full, playerPtr) {}
bool HpSensitiveBuilding::works() const {
	return (this->getHP() == this->getMaxHP());
}
GameObjectResponse HpSensitiveBuilding::processRegeneration() {
	bool works1 = this->works();
	this->Building::processRegeneration();
	bool works2 = this->works();
	GameObjectResponse response;
	if (works2 and !works1) {
		MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", this->getBuildingFinishedStr());
		response.popUpWindows.push(window);
	}
	return response;
}
GameObjectResponse HpSensitiveBuilding::handleDoesNotWork() const {
	GameObjectResponse response;
	MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", this->getIsNotBuiltYetStr());
	response.popUpWindows.push(window);
	return response;
}