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



#include "Sawmill.hpp"
#include "Texts.hpp"


Sawmill::Sawmill() = default;
Sawmill::Sawmill(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
	ResourceB(x, y, 2, 2, 5000, playerId, units, resourcePointsPtr, go, mapW, mapH),
	Building(x, y, 2, 2, 5000, playerId, units) {}
Building* Sawmill::cloneBuilding() const {
	return new Sawmill(*this);
}
Resources Sawmill::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 21000));
	return cost;
}
uint32_t Sawmill::getRegenerationSpeed() const {
	return 1750;
}
std::string Sawmill::getTextureName() const {
	return "sawmill";
}
std::string Sawmill::getSoundName() const {
	return "wood";
}
std::string Sawmill::getResourceType() const {
	return "wood";
}
uint32_t Sawmill::getCollectionSpeed() const {
	return 3000;
}
uint32_t Sawmill::getRadius() const {
	return 5;
}
std::wstring Sawmill::getDescription() const {
	return *Texts::get()->get("sawmill_description");
}
std::wstring Sawmill::getUpperCaseReadableName() const {
	return *Texts::get()->get("sawmill_upper_case_readable_name");
}