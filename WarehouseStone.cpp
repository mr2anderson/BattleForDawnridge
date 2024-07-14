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


#include "WarehouseStone.hpp"
#include "Texts.hpp"


WarehouseStone::WarehouseStone() = default;
WarehouseStone::WarehouseStone(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	ResourceStorageB(x, y, playerId, units),
	Building(x, y, playerId, units) {

}
Building* WarehouseStone::cloneBuilding() const {
	return new WarehouseStone(*this);
}
uint32_t WarehouseStone::getSX() const {
    return 2;
}
uint32_t WarehouseStone::getSY() const {
    return 2;
}
uint32_t WarehouseStone::getMaxHP() const {
    return 5000;
}
Events WarehouseStone::newMove(uint32_t playerId) {
	Events response;
	if (this->belongTo(playerId) and this->exist()) {
		return  this->regenerate();
	}
	return response;
}
Defence WarehouseStone::getDefence() const {
	return DEFENCE::STONE;
}
Resources WarehouseStone::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 10000));
	return cost;
}
uint32_t WarehouseStone::getRegenerationSpeed() const {
	return 1000;
}
std::string WarehouseStone::getTextureName() const {
	return "warehouse_stone";
}
std::string WarehouseStone::getSoundName() const {
	return "stone";
}
std::wstring WarehouseStone::getDescription() const {
	return *Texts::get()->get("warehouse_stone_description");
}
Resources WarehouseStone::getLimit() const {
	return Resources({ Resource("stone", 20000) });
}
std::wstring WarehouseStone::getUpperCaseReadableName() const {
	return *Texts::get()->get("warehouse_stone_upper_case_readable_name");
}
Events WarehouseStone::getSelectionW() {
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
Events WarehouseStone::getGameObjectResponse(uint32_t playerId) {
	if (this->exist() and this->belongTo(playerId)) {
		return this->getSelectionW();
	}
	return Events();
}