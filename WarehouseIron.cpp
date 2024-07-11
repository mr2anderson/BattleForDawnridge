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


#include "WarehouseIron.hpp"
#include "Texts.hpp"


WarehouseIron::WarehouseIron() = default;
WarehouseIron::WarehouseIron(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	ResourceStorageB(x, y, 3, 3, 20000, playerId, units),
	Building(x, y, 3, 3, 20000, playerId, units) {

}
Building* WarehouseIron::cloneBuilding() const {
	return new WarehouseIron(*this);
}
Events WarehouseIron::newMove(uint32_t playerId) {
	Events response;
	if (this->belongTo(playerId) and this->exist()) {
		return  this->regenerate();
	}
	return response;
}
Resources WarehouseIron::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 20000));
	return cost;
}
uint32_t WarehouseIron::getRegenerationSpeed() const {
	return 5;
}
std::string WarehouseIron::getTextureName() const {
	return "warehouse_iron";
}
std::string WarehouseIron::getSoundName() const {
	return "iron";
}
std::wstring WarehouseIron::getDescription() const {
	return *Texts::get()->get("warehouse_iron_description");
}
Resources WarehouseIron::getLimit() const {
	return Resources({ Resource("iron", 10000) });
}
std::wstring WarehouseIron::getUpperCaseReadableName() const {
	return *Texts::get()->get("warehouse_iron_upper_case_readable_name");
}
Events WarehouseIron::getSelectionW() {
	Events response;

	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	components.push_back(this->getResourceStorageComponent());
	components.push_back(this->getDestroyComponent());

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events WarehouseIron::getGameObjectResponse(uint32_t playerId) {
	if (this->belongTo(playerId)) {
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}