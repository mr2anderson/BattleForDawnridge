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


#include "Road.hpp"
#include "Texts.hpp"


Road::Road() = default;
Road::Road(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<TerritoryOriginB>> originsPtr) :
	TerritoryConductorB(x, y, 1, 1, 500, playerId, units, originsPtr),
	Building(x, y, 1, 1, 500, playerId, units) {
	
}
Building* Road::cloneBuilding() const {
	return new Road(*this);
}
Events Road::newMove(uint32_t playerId) {
	if (this->belongTo(playerId) and this->exist()) {
		return this->regenerate();
	}
	return Events();
}
Resources Road::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 1500));
	return cost;
}
uint32_t Road::getRegenerationSpeed() const {
	return 500;
}
std::string Road::getTextureName() const {
	return "road";
}
std::string Road::getSoundName() const {
	return "road";
}
std::wstring Road::getDescription() const {
	return *Texts::get()->get("road_description");
}
std::wstring Road::getUpperCaseReadableName() const {
	return *Texts::get()->get("road_upper_case_readable_name");
}
uint32_t Road::getRadius() const {
	return 2;
}
Events Road::getSelectionW() {
	Events response;

	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	components.push_back(this->getDestroyComponent());
	if (this->repairing()) {
		components.push_back(this->getBusyWithRepairingComponent());
	}
	if (this->works()) {
		if (!this->connectedToTerritoryOriginB()) {
			components.push_back(this->getNotConnectedComponent());
		}
	}

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events Road::getGameObjectResponse(uint32_t playerId) {
	if (!this->exist()) {
		return Events();
	}
	if (this->belongTo(playerId)) {
		return this->getHighlightEvent() + this->getSelectionW();
	}
	return Events();
}