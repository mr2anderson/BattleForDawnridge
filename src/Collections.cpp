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


void Collections::add(std::shared_ptr<GO>object) {
    this->gos.push(object);
    this->addToSubClassCollections(object);
}


uint32_t Collections::totalGOs() const {
	return this->gos.size();
}
uint32_t Collections::totalAreaRPs() const {
	return this->areaRps.size();
}
uint32_t Collections::totalConductionRPs() const {
    return this->conductionRps.size();
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


std::shared_ptr<GO> Collections::getGO(uint32_t i, uint8_t filter) {
	return this->gos.at(i, filter);
}
std::shared_ptr<AreaResourcePoint> Collections::getAreaRP(uint32_t i, uint8_t filter) {
	return this->areaRps.at(i, filter);
}
std::shared_ptr<ConductionResourcePoint> Collections::getConductionRP(uint32_t i, uint8_t filter) {
    return this->conductionRps.at(i, filter);
}
std::shared_ptr<Unit> Collections::getUnit(uint32_t i, uint8_t filter) {
    return this->units.at(i, filter);
}
std::shared_ptr<Building>  Collections::getBuilding(uint32_t i, uint8_t filter) {
	return this->buildings.at(i, filter);
}
std::shared_ptr<Warrior>  Collections::getWarrior(uint32_t i, uint8_t filter)  {
	return this->warriors.at(i, filter);
}


void Collections::clearSubClassCollections() {
	this->areaRps.clear();
	this->conductionRps.clear();
	this->units.clear();
	this->buildings.clear();
	this->warriors.clear();
}
void Collections::addToSubClassCollections(std::shared_ptr<GO> object) {
	if (std::shared_ptr<AreaResourcePoint> areaRp = std::dynamic_pointer_cast<AreaResourcePoint>(object)) {
		this->areaRps.push(areaRp);
	}
	if (std::shared_ptr<ConductionResourcePoint> conductionRp = std::dynamic_pointer_cast<ConductionResourcePoint>(object)) {
		this->conductionRps.push(conductionRp);
	}
	if (std::shared_ptr<Unit> u = std::dynamic_pointer_cast<Unit>(object)) {
		this->units.push(u);
	}
	if (std::shared_ptr<Building>  b = std::dynamic_pointer_cast<Building>(object)) {
		this->buildings.push(b);
	}
	if (std::shared_ptr<Warrior>  w = std::dynamic_pointer_cast<Warrior>(object)) {
		this->warriors.push(w);
	}
}


BOOST_CLASS_EXPORT_IMPLEMENT(Collections)