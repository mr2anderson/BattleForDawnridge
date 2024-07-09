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


WarehouseStone::WarehouseStone() = default;
WarehouseStone::WarehouseStone(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, 2, 2, 30000, playerId) {

}
Building* WarehouseStone::cloneBuilding() const {
	return new WarehouseStone(*this);
}
Events WarehouseStone::newMove(uint32_t playerId) {
	Events response;
	if (this->belongTo(playerId) and this->exist()) {
		return  this->regenerate();
	}
	return response;
}
Resources WarehouseStone::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 20000));
	return cost;
}
uint32_t WarehouseStone::getRegenerationSpeed() const {
	return 10000;
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
	return Resources({ Resource("stone", 10000) });
}
std::wstring WarehouseStone::getUpperCaseReadableName() const {
	return *Texts::get()->get("warehouse_stone_upper_case_readable_name");
}
Events WarehouseStone::getSelectionW() {
	Events response;

	std::vector<GameActionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	components.push_back(this->getResourceStorageComponent());

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events WarehouseStone::getGameObjectResponse(uint32_t playerId) {
	if (this->belongTo(playerId)) {
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}