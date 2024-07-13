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


#include "ImpassableObstacle.hpp"
#include "CreateEEvent.hpp"


ImpassableObstacle::ImpassableObstacle() = default;
ImpassableObstacle::ImpassableObstacle(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) :
	GO(x, y, sx, sy) {

}
Events ImpassableObstacle::newMove(uint32_t playerId) {
	return Events();
}
bool ImpassableObstacle::exist() const {
	return true;
}
bool ImpassableObstacle::warriorCanStay(uint32_t warriorPlayerId) const {
	return false;
}
uint32_t ImpassableObstacle::getWarriorMovementCost(uint32_t warriorPlayerId) const {
	return WARRIOR_MOVEMENT_FORBIDDEN;
}
Events ImpassableObstacle::getGameObjectResponse(uint32_t playerId) {
	return this->getHorizontalSelectionWindow();
}
Events ImpassableObstacle::getHorizontalSelectionWindow() {
	Events events;

	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(this->getSoundName(), "click", components);
	events.add(std::make_shared<CreateEEvent>(window));

	return events;
}