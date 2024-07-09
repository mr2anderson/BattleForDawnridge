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


#include "Castle.hpp"


Castle::Castle() = default;
Castle::Castle(uint32_t x, uint32_t y, uint32_t playerId) : 
	TerritoryOriginB(x, y, 3, 3, 100, playerId),
	Building(x, y, 3, 3, 100000, playerId) {
}
Building* Castle::cloneBuilding() const {
	return new Castle(*this);
}
Events Castle::newMove(uint32_t playerId) {
	Events response;
	if (this->belongTo(playerId) and this->exist()) {
		return this->regenerate();
	}
	return response;
}
Resources Castle::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 100000));
	return cost;
}
std::string Castle::getTextureName() const {
	return "castle";
}
std::string Castle::getSoundName() const {
	return "hooray";
}
std::wstring Castle::getDescription() const {
	return *Texts::get()->get("castle_description");
}
uint32_t Castle::getRegenerationSpeed() const {
	return 25000;
}
std::wstring Castle::getUpperCaseReadableName() const {
	return *Texts::get()->get("castle_upper_case_readable_name");
}
uint32_t Castle::getRadius() const {
	return 3;
}
Events Castle::getSelectionW() {
	Events response;

	std::vector<GameActionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	if (this->repairing()) {
		components.push_back(this->getBusyWithRepairingComponent());
	}

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events Castle::getGameObjectResponse(uint32_t playerId) {
	if (!this->exist()) {
		return Events();
	}
	if (this->belongTo(playerId)) {
		return this->getHighlightEvent() + this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}