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
#include "CollectionsImpl.hpp"


Collections::Collections() {
    this->impl = std::make_unique<CollectionsImpl>();
}
Collections::~Collections() = default;


void Collections::add(GO *object) {
    this->impl->gos.push(object);
    this->addToSubClassCollections(object);
}


uint32_t Collections::totalGOs() const {
	return this->impl->gos.size();
}
uint32_t Collections::totalAreaRPs() const {
	return this->impl->areaRps.size();
}
uint32_t Collections::totalConductionRPs() const {
    return this->impl->conductionRps.size();
}
uint32_t Collections::totalUnits() const {
    return this->impl->units.size();
}
uint32_t Collections::totalBuildings() const {
	return this->impl->buildings.size();
}
uint32_t Collections::totalWarriors() const {
	return this->impl->warriors.size();
}


GO* Collections::getGO(uint32_t i, uint8_t filter) {
	return this->impl->gos.at(i, filter);
}
const GO* Collections::getGO(uint32_t i, uint8_t filter) const {
    return this->impl->gos.at(i, filter);
}
AreaResourcePoint* Collections::getAreaRP(uint32_t i) {
	return this->impl->areaRps.at(i);
}
ConductionResourcePoint* Collections::getConductionRP(uint32_t i) {
    return this->impl->conductionRps.at(i);
}
Unit* Collections::getUnit(uint32_t i) {
    return this->impl->units.at(i);
}
Building* Collections::getBuilding(uint32_t i) {
	return this->impl->buildings.at(i);
}
Warrior* Collections::getWarrior(uint32_t i)  {
	return this->impl->warriors.at(i);
}
void Collections::clearSubClassCollections() {
    this->impl->areaRps.clear();
    this->impl->conductionRps.clear();
    this->impl->units.clear();
    this->impl->buildings.clear();
    this->impl->warriors.clear();
}
void Collections::addToSubClassCollections(GO *object) {
    if (AreaResourcePoint* areaRp = dynamic_cast<AreaResourcePoint*>(object)) {
        this->impl->areaRps.push(areaRp);
    }
    if (ConductionResourcePoint* conductionRp = dynamic_cast<ConductionResourcePoint*>(object)) {
        this->impl->conductionRps.push(conductionRp);
    }
    if (Unit* u = dynamic_cast<Unit*>(object)) {
        this->impl->units.push(u);
    }
    if (Building* b = dynamic_cast<Building*>(object)) {
        this->impl->buildings.push(b);
    }
    if (Warrior* w = dynamic_cast<Warrior*>(object)) {
        this->impl->warriors.push(w);
    }
}


template<class Archive> void Collections::serialize(Archive& ar, const unsigned int version) {
    ar& this->impl->gos;
    if (Archive::is_loading::value) {
        this->clearSubClassCollections();
        for (uint32_t i = 0; i < this->impl->gos.size(); i = i + 1) {
            this->addToSubClassCollections(this->impl->gos.at(i, FILTER::DEFAULT_PRIORITY));
        }
    }
}
template void Collections::serialize(iarchive& ar, const unsigned int version);
template void Collections::serialize(oarchive& ar, const unsigned int version);