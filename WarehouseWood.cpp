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


#include "WarehouseWood.hpp"
#include "Texts.hpp"
#include "CreateEEvent.hpp"


WarehouseWood::WarehouseWood() = default;
WarehouseWood::WarehouseWood(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	ResourceStorageB(x, y, playerId, units),
	Building(x, y, playerId, units) {

}
Building* WarehouseWood::cloneBuilding() const {
	return new WarehouseWood(*this);
}
uint32_t WarehouseWood::getSX() const {
    return 2;
}
uint32_t WarehouseWood::getSY() const {
    return 2;
}
uint32_t WarehouseWood::getMaxHP() const {
    return 10000;
}
Events WarehouseWood::newMove(uint32_t playerId) {
	Events response;
	if (this->belongTo(playerId) and this->exist()) {
		return  this->regenerate();
	}
	return response;
}
Defence WarehouseWood::getDefence() const {
	return Defence::WOOD;
}
Resources WarehouseWood::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 10000));
	return cost;
}
uint32_t WarehouseWood::getRegenerationSpeed() const {
	return this->getMaxHP() / 5;
}
std::string WarehouseWood::getTextureName() const {
	return "warehouse_wood";
}
std::string WarehouseWood::getSoundName() const {
	return "wood";
}
std::wstring WarehouseWood::getDescription() const {
	return *Texts::get()->get("warehouse_wood_description");
}
Resources WarehouseWood::getLimit() const {
	return Resources({ Resource("wood", 20000) });
}
std::wstring WarehouseWood::getUpperCaseReadableName() const {
	return *Texts::get()->get("warehouse_wood_upper_case_readable_name");
}
Events WarehouseWood::getSelectionW() {
	Events response;

	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	components.push_back(this->getResourceStorageComponent());
	components.push_back(this->getDestroyComponent());

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);
    response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events WarehouseWood::getGameObjectResponse(uint32_t playerId) {
	if (this->exist() and this->belongTo(playerId)) {
		return this->getSelectionW();
	}
	return Events();
}