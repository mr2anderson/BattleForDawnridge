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


#include "Collections.hpp"


Collections::Collections() = default;


void Collections::addToGOs(GO* go) {
	this->gos.push(go);
}
void Collections::addToEffects(Effect* effect) {
	this->effects.push(effect);
}
void Collections::addToRPs(ResourcePoint* rp) {
	this->rps.push(rp);
}
void Collections::addToUnits(Unit *u) {
    this->units.push(u);
}
void Collections::addToBuildings(Building* b) {
	this->buildings.push(b);
}
void Collections::addToWarriors(Warrior* w) {
	this->warriors.push(w);
}


uint32_t Collections::totalGOs() const {
	return this->gos.size();
}
uint32_t Collections::totalEffects() const {
	return this->effects.size();
}
uint32_t Collections::totalRPs() const {
	return this->rps.size();
}
uint32_t Collections::totalUnits() const {
    return this->units.size();
}
uint32_t Collections::totalBuildings() const {
	return this->buildings.size();
}
uint32_t Collections::totalWarriors() const {
	return this->warriors.size();
}


GO* Collections::getGO(uint32_t i) {
	return this->gos.at(i);
}
Effect* Collections::getEffect(uint32_t i) {
	return this->effects.at(i);
}
ResourcePoint* Collections::getRP(uint32_t i) {
	return this->rps.at(i);
}
Unit* Collections::getUnit(uint32_t i) {
    return this->units.at(i);
}
Building* Collections::getBuilding(uint32_t i) {
	return this->buildings.at(i);
}
Warrior* Collections::getWarrior(uint32_t i) {
	return this->warriors.at(i);
}


const GO* Collections::getGO(uint32_t i) const {
	return this->gos.at(i);
}