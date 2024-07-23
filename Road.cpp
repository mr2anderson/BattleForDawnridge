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
#include "RoadSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "ReconfRoadEvent.hpp"


Road::Road() {
    this->type = "none";
}
Road::Road(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {

    this->type = "none";

	this->addSpec(new RoadSpec());
}
Building* Road::createSameTypeBuilding() const {
	return new Road(this->getX(), this->getY(), this->getPlayerId());
}
Events Road::newFrame(MapState *state, uint32_t playerId) {
    Events events;

    if (this->exist() and this->works()) {
        std::string properType = this->getProperType(state);
        if (this->type != properType) {
            events.add(std::make_shared<ReconfRoadEvent>(this, properType));
        }
    }

    return events;
}
void Road::reconf(const std::string &properType) {
    this->type = properType;
}
uint32_t Road::getSX() const { // Config file is not used cuz engine does not support other sizes
	return 1;
}
uint32_t Road::getSY() const {
	return 1;
}
uint32_t Road::getMaxHP() const {
	return Parameters::get()->getInt("road_max_hp");
}
Defence Road::getDefence() const {
	return Parameters::get()->getDefence("road_defence");
}
Resources Road::getCost() const {
	return Parameters::get()->getResources("road_cost");
}
uint32_t Road::getRegenerationSpeed() const {
	return Parameters::get()->getInt("road_regeneration_speed");
}
std::string Road::getTextureName() const {
	return "road_" + this->type;
}
std::string Road::getSoundName() const {
	return "road";
}
std::wstring Road::getDescription() const {
	return *Locales::get()->get("road_description");
}
std::string Road::getProperType(MapState *state) const {
    bool horizontal = false;
    bool vertical = false;

    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
        Building *b = state->getCollectionsPtr()->getBuilding(i);
        if (b->exist() and b->getPlayerId() == this->getPlayerId() and (b->isActiveConductor() or b->isOrigin())) {
            if (this->getY() >= b->getY() and this->getY() < b->getY() + b->getSY() and (b->getX() + b->getSX() == this->getX() or this->getX() + this->getSX() == b->getX())) {
                horizontal = true;
            }
            if (this->getX() >= b->getX() and this->getX() < b->getX() + b->getSX() and (b->getY() + b->getSY() == this->getY() or this->getY() + this->getSY() == b->getY())) {
                vertical = true;
            }
        }
    }

    if (horizontal and vertical) {
        return "all";
    }
    if (horizontal) {
        return "horizontal";
    }
    if (vertical) {
        return "vertical";
    }
    return "none";
}