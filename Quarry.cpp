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



#include "Quarry.hpp"
#include "Texts.hpp"


Quarry::Quarry() = default;
Quarry::Quarry(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
	ResourceB(x, y, 2, 2, 5000, playerId, units, resourcePointsPtr, go, mapW, mapH),
	Building(x, y, 2, 2, 5000, playerId, units) {}
Building* Quarry::cloneBuilding() const {
	return new Quarry(*this);
}
Resources Quarry::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 20000));
	return cost;
}
uint32_t Quarry::getRegenerationSpeed() const {
	return 1750;
}
std::string Quarry::getTextureName() const {
	return "quarry";
}
std::string Quarry::getSoundName() const {
	return "stone";
}
std::string Quarry::getResourceType() const {
	return "stone";
}
uint32_t Quarry::getCollectionSpeed() const {
	return 1500;
}
uint32_t Quarry::getRadius() const {
	return 4;
}
std::wstring Quarry::getDescription() const {
	return *Texts::get()->get("quarry_description");
}
std::wstring Quarry::getUpperCaseReadableName() const {
	return *Texts::get()->get("quarry_upper_case_readable_name");
}