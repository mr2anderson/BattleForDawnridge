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


#include "TerritoryConductorB.hpp"
#include "Texts.hpp"


TerritoryConductorB::TerritoryConductorB() = default;
TerritoryConductorB::TerritoryConductorB(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<Collection<Unit>> units, std::shared_ptr<Collection<GO>> go, uint32_t mapW, uint32_t mapH, std::shared_ptr<Collection<TerritoryOriginB>> originsPtr) :
	TerritoryB(x, y, playerId, units, go, mapW, mapH),
	ConductorB(x, y, playerId, units),
	Building(x, y, playerId, units) {
	this->originsPtr = originsPtr;
}
bool TerritoryConductorB::allowBuilding(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId) {
	if (!this->connectedToTerritoryOriginB()) {
		return false;
	}
	return this->TerritoryB::allowBuilding(x, y, sx, sy, playerId);
}
Events TerritoryConductorB::getHighlightEvent() {
	if (!this->connectedToTerritoryOriginB()) {
		return Events();
	}
	return this->TerritoryB::getHighlightEvent();
}
bool TerritoryConductorB::connectedToTerritoryOriginB() const {
	for (uint32_t i = 0; i < this->originsPtr->size(); i = i + 1) {
		if (this->connectedTo(this->originsPtr->at(i))) {
			return true;
		}
	}
	return false;
}
HorizontalSelectionWindowComponent TerritoryConductorB::getNotConnectedComponent() const {
	HorizontalSelectionWindowComponent component = {
		this->getTextureName(),
		*Texts::get()->get("does_not_lead_to_city_center"),
		false,
		Events()
	};
	return component;
}