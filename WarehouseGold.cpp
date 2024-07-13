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


#include "WarehouseGold.hpp"
#include "Texts.hpp"
#include "TryToCollectEvent.hpp"


WarehouseGold::WarehouseGold() = default;
WarehouseGold::WarehouseGold(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<Treasure>> treasures) :
	ResourceStorageB(x, y, 2, 2, 10000, playerId, units),
	Building(x, y, 2, 2, 10000, playerId, units) {
	this->treasures = treasures;
}
Building* WarehouseGold::cloneBuilding() const {
	return new WarehouseGold(*this);
}
Events WarehouseGold::newMove(uint32_t playerId) {
	Events response;
	if (this->belongTo(playerId) and this->exist()) {
		return this->regenerate() + this->collect();
	}
	return response;
}
Resources WarehouseGold::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 20000));
	return cost;
}
uint32_t WarehouseGold::getRegenerationSpeed() const {
	return 2500;
}
std::string WarehouseGold::getTextureName() const {
	return "warehouse_gold";
}
std::string WarehouseGold::getSoundName() const {
	return "gold";
}
std::wstring WarehouseGold::getDescription() const {
	return *Texts::get()->get("warehouse_gold_description");
}
Resources WarehouseGold::getLimit() const {
	return Resources({ Resource("gold", 10000) });
}
std::wstring WarehouseGold::getUpperCaseReadableName() const {
	return *Texts::get()->get("warehouse_gold_upper_case_readable_name");
}
Events WarehouseGold::collect() {
	Events events;

	for (uint32_t i = 0; i < this->treasures->size(); i = i + 1) {
		Treasure* t = this->treasures->at(i);
		if (t->exist() and this->connectedTo(t)) {
			events.add(std::make_shared<TryToCollectEvent>(this->getPlayerId(), t, 2000));
		}
	}

	return events;
}
Events WarehouseGold::getSelectionW() {
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
Events WarehouseGold::getGameObjectResponse(uint32_t playerId) {
	if (this->exist() and this->belongTo(playerId)) {
		return this->getSelectionW();
	}
	return Events();
}