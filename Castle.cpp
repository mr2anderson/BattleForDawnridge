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
#include "Texts.hpp"


Castle::Castle() = default;
Castle::Castle(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	TerritoryOriginB(x, y, playerId, units),
	ResourceStorageB(x, y,  playerId, units),
	VictoryConditionB(x, y, playerId, units),
	Building(x, y, playerId, units) {
}
Building* Castle::cloneBuilding() const {
	return new Castle(*this);
}
Events Castle::newMove(uint32_t playerId) {
	Events response;
	if (this->belongTo(playerId) and this->exist()) {
		return this->regenerate() + this->addWood();
	}
	return response;
}
uint32_t Castle::getSX() const {
    return 2;
}
uint32_t Castle::getSY() const {
    return 2;
}
uint32_t Castle::getMaxHP() const {
    return 50000;
}
Resources Castle::getLimit() const {
	return Resources({ Resource("food", 20000), Resource("wood", 20000), Resource("stone", 20000)});
}
Defence Castle::getDefence() const {
	return DEFENCE::STONE;
}
Resources Castle::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 50000));
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
Events Castle::destroy() {
	return this->ResourceStorageB::destroy() + this->VictoryConditionB::destroy();
}
uint32_t Castle::getRegenerationSpeed() const {
	return 10000;
}
std::wstring Castle::getUpperCaseReadableName() const {
	return *Texts::get()->get("castle_upper_case_readable_name");
}
uint32_t Castle::getRadius() const {
	return 3;
}
Events Castle::getSelectionW() {
	Events response;

	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	components.push_back(this->getVictoryConditionComponent());
	components.push_back(this->getResourceStorageComponent());
	components.push_back(this->getDestroyComponent());
	if (this->repairing()) {
		components.push_back(this->getBusyWithRepairingComponent());
	}

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);
    response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
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
	return Events();
}