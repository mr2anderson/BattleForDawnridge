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



TerritoryConductorB::TerritoryConductorB() = default;
TerritoryConductorB::TerritoryConductorB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, Player* player, std::vector<TerritoryOriginB*>* originsPtr, std::vector<TerritoryConductorB*>* conductorsPtr) :
	TerritoryB(x, y, sx, sy, maxHp, player),
	Building(x, y, sx, sy, maxHp, player) {
	this->originsPtr = originsPtr;
	this->conductorsPtr = conductorsPtr;
}
bool TerritoryConductorB::allowBuilding(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, Player *player) const {
	if (!this->conducted()) {
		return false;
	}
	return this->TerritoryB::allowBuilding(x, y, sx, sy, player);
}
Event TerritoryConductorB::getHighlightEvent() const {
	if (!this->conducted()) {
		return Event();
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
		if (o->belongTo(this->getPlayer()) and o->works()) {
			g.addOrigin(o->getX(), o->getY(), o->getSX(), o->getSY());
		}
	}
	for (uint32_t i = 0; i < this->conductorsPtr->size(); i = i + 1) {
		auto c = this->conductorsPtr->at(i);
		if (c->belongTo(this->getPlayer()) and c->works()) {
			g.addConductor(c->getX(), c->getY(), c->getSX(), c->getSY());
		}
	}
	return g.connectedToOrigin(this->getX(), this->getY());
}
Event TerritoryConductorB::getNotConductedResponce() const {
	MessageW* w = new MessageW(this->getSoundName(), "click",
		this->getUpperCaseReadableName() + L" ме бедер мх й ндмнлс цнпндяйнлс жемрпс\n");
	Event events;
	events.createE.push_back(w);
	return events;
}