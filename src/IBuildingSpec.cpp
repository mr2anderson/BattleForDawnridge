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


#include "IBuildingSpec.hpp"
#include "GO.hpp"


Events IBuildingSpec::getActiveNewMoveEvent(std::shared_ptr<const Building> building, MapState* state) {
	return Events();
}
Events IBuildingSpec::getHighlightEvent(std::shared_ptr<const Building> building, MapState* state, uint8_t type) const {
	return Events();
}
Events IBuildingSpec::getEventOnDestroy(std::shared_ptr<const Building> building, MapState* state) const {
	return Events();
}
std::vector<BuildingHorizontalSelectionWindowComponent> IBuildingSpec::getComponents(std::shared_ptr<const Building> building, MapState* state) {
	return std::vector<BuildingHorizontalSelectionWindowComponent>();
}
boost::optional<BuildingShortInfo> IBuildingSpec::getShortInfo(std::shared_ptr<const Building> building) const {
	return boost::none;
}
Resources IBuildingSpec::getLimit(std::shared_ptr<const Building> building) const {
	return Resources();
}
uint32_t IBuildingSpec::getPopulationLimit(std::shared_ptr<const Building> building) const {
	return 0;
}
bool IBuildingSpec::isVictoryCondition() const {
	return false;
}
bool IBuildingSpec::isOrigin() const {
	return false;
}
bool IBuildingSpec::isActiveConductor(std::shared_ptr<const Building> building) const {
	return false;
}
uint32_t IBuildingSpec::getWarriorMovementCost(std::shared_ptr<const Building> building, std::shared_ptr<Warrior> w) const {
	return 1;
}
bool IBuildingSpec::warriorCanStay(std::shared_ptr<const Building> building, std::shared_ptr<const Warrior> w) const {
	return true;
}
bool IBuildingSpec::isUltraHighObstacle(std::shared_ptr<const Building> building, uint32_t playerId) const {
	return false;
}
bool IBuildingSpec::isHighObstacle(std::shared_ptr<const Building> building, uint32_t playerId) const {
	return false;
}
bool IBuildingSpec::isLowObstacle(std::shared_ptr<const Building> building, uint32_t playerId) const {
	return false;
}
NewMoveMainPriority IBuildingSpec::getNewMoveMainPriority() const {
    return GO::NEW_MOVE_MAIN_PRIORITY_LOWEST;
}


BOOST_CLASS_EXPORT_IMPLEMENT(IBuildingSpec)