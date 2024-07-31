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
#include "GO.hpp"
#include "Effect.hpp"
#include "ResourcePoint.hpp"
#include "Unit.hpp"
#include "Building.hpp"
#include "Warrior.hpp"


Collections::Collections() = default;


void Collections::add(GO *object) {
    this->gos.push(object);

    if (Effect* effect = dynamic_cast<Effect*>(object)) {
        this->effects.push(effect);
    }
    if (ResourcePoint* rp = dynamic_cast<ResourcePoint*>(object)) {
        this->rps.push(rp);
    }
    if (Unit* u = dynamic_cast<Unit*>(object)) {
        this->units.push(u);
    }
    if (Building* b = dynamic_cast<Building*>(object)) {
        this->buildings.push(b);
    }
    if (Warrior* w = dynamic_cast<Warrior*>(object)) {
        this->warriors.push(w);
    }
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


template<class Archive> void Collections::serialize(Archive &ar, const unsigned int version) {
    ar & this->gos.data;
    ar & this->gos.dataIndexed;

    ar & this->effects.data;
    ar & this->effects.dataIndexed;

    ar & this->rps.data;
    ar & this->rps.dataIndexed;

    ar & this->units.data;
    ar & this->units.dataIndexed;

    ar & this->buildings.data;
    ar & this->buildings.dataIndexed;

    ar & this->warriors.data;
    ar & this->warriors.dataIndexed;
}


BOOST_CLASS_EXPORT_IMPLEMENT(Collections)


template void Collections::serialize(iarchive &ar, const unsigned int version);
template void Collections::serialize(oarchive &ar, const unsigned int version);