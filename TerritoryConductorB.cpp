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
#include "ConductionGraph.hpp"
#include "Texts.hpp"


TerritoryConductorB::TerritoryConductorB() = default;
TerritoryConductorB::TerritoryConductorB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<TerritoryOriginB>> originsPtr, std::shared_ptr<GOCollection<TerritoryConductorB>> conductorsPtr) :
	TerritoryB(x, y, sx, sy, maxHp, playerId),
	Building(x, y, sx, sy, maxHp, playerId) {
	this->originsPtr = originsPtr;
	this->conductorsPtr = conductorsPtr;
}
bool TerritoryConductorB::allowBuilding(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId) const {
	if (!this->conducted()) {
		return false;
	}
	return this->TerritoryB::allowBuilding(x, y, sx, sy, playerId);
}
Events TerritoryConductorB::getHighlightEvent() const {
	if (!this->conducted()) {
		return Events();
	}
	return this->TerritoryB::getHighlightEvent();
}
bool TerritoryConductorB::conducted() const {
	if (!this->works()) {
		return false;
	}
	ConductionGraph g;
	for (uint32_t i = 0; i < this->originsPtr->size(); i = i + 1) {
		auto o = this->originsPtr->at(i);
		if (o->belongTo(this->getPlayerId()) and o->works()) {
			g.addOrigin(o->getX(), o->getY(), o->getSX(), o->getSY());
		}
	}
	for (uint32_t i = 0; i < this->conductorsPtr->size(); i = i + 1) {
		auto c = this->conductorsPtr->at(i);
		if (c->belongTo(this->getPlayerId()) and c->works()) {
			g.addConductor(c->getX(), c->getY(), c->getSX(), c->getSY());
		}
	}
	return g.connectedToOrigin(this->getX(), this->getY());
}
HorizontalSelectionWindowComponent TerritoryConductorB::getNotConductedComponent() const {
	HorizontalSelectionWindowComponent component = {
		this->getTextureName(),
		*Texts::get()->get("does_not_lead_to_city_center"),
		false,
		false,
		Events()
	};
	return component;
}