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


Road::Road() = default;
Road::Road(uint32_t x, uint32_t y, Player* playerPtr, std::vector<TerritoryOriginB*>* originsPtr, std::vector<TerritoryConductorB*>* conductorsPtr) :
	TerritoryConductorB(x, y, 1, 1, 1000, playerPtr, originsPtr, conductorsPtr),
	Building(x, y, 1, 1, 1000, playerPtr) {
	
}
Building* Road::clone() const {
	return new Road(*this);
}
Event Road::newMove(Player* player) {
	if (this->belongTo(player) and this->exist()) {
		return this->regenerate();
	}
	return Event();
}
Resources Road::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 1000));
	return cost;
}
uint32_t Road::getRegenerationSpeed() const {
	return 1000;
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
	return 3;
}
Event Road::getSelectionW() {
	Event response;

	std::vector<GameActionWindowComponent> components;
	components.emplace_back("exit_icon", *Texts::get()->get("leave"), true, true, this->getHighlightEvent());
	components.emplace_back("road",
		this->getDescription() + L'\n'
		+ this->getReadableHpInfo(), false, false, Event());

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.addCreateEEvent(window);

	return response;
}
Event Road::getGameObjectResponse(Player* player) {
	if (!this->exist()) {
		return Event();
	}
	if (this->belongTo(player)) {
		if (this->repairing()) {
			return this->handleRepairing();
		}
		if (!this->conducted()) {
			return this->getNotConductedResponce();
		}
		Event responce;
		responce = this->getHighlightEvent();
		return responce + this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}