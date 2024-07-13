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


#include "Wall.hpp"


Wall::Wall() = default;
Wall::Wall(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Building(x, y, sx, sy, maxHp, playerId, units) {

}
Events Wall::newMove(uint32_t playerId) {
	Events response;
	if (this->belongTo(playerId) and this->exist()) {
		return this->regenerate();
	}
	return response;
}
bool Wall::warriorCanStay(uint32_t warriorPlayerId) const {
	return false;
}
uint32_t Wall::getWarriorMovementCost(uint32_t warriorPlayerId) const {
	return WARRIOR_MOVEMENT_FORBIDDEN;
}
Events Wall::getSelectionW() {
	Events response;

	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	components.push_back(this->getDestroyComponent());

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events Wall::getGameObjectResponse(uint32_t playerId) {
	if (this->belongTo(playerId)) {
		return this->getSelectionW();
	}
	return Events();
}