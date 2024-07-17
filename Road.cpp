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
#include "CreateEEvent.hpp"


Road::Road() = default;
Road::Road(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<Collection<Unit>> units, std::shared_ptr<Collection<GO>> go, uint32_t mapW, uint32_t mapH, std::shared_ptr<Collection<TerritoryOriginB>> originsPtr) :
	TerritoryConductorB(x, y, playerId, units, go, mapW, mapH, originsPtr),
	HpSensitiveB(x, y, playerId, units),
	Building(x, y, playerId, units) {
	
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
uint32_t Road::getSX() const {
    return 1;
}
uint32_t Road::getSY() const {
    return 1;
}
uint32_t Road::getMaxHP() const {
    return 500;
}
Defence Road::getDefence() const {
	return Defence::WOOD;
}
Resources Road::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 1500));
	return cost;
}
uint32_t Road::getRegenerationSpeed() const {
	return this->getMaxHP();
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
	return 4;
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
	if (this->works() and !this->connectedToTerritoryOriginB()) {
		components.push_back(this->getNotConnectedComponent());
	}

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);
    response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
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