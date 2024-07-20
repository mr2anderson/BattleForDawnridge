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


#include "Castle.hpp"
#include "CastleTerritoryExpanderSpec.hpp"
#include "CastleWarehouseSpec.hpp"
#include "VictoryConditionSpec.hpp"
#include "CastleWoodGeneratorSpec.hpp"
#include "CastlePopulationLimitIncreaserSpec.hpp"
#include "Texts.hpp"


Castle::Castle() = default;
Castle::Castle(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new CastleTerritoryExpanderSpec());
	this->addSpec(new CastleWoodGeneratorSpec());
	this->addSpec(new CastleWarehouseSpec());
	this->addSpec(new VictoryConditionSpec());
	this->addSpec(new CastlePopulationLimitIncreaserSpec());
}
Building* Castle::cloneBuilding() const {
	return new Castle(*this);
}
uint32_t Castle::getSX() const {
    return 2;
}
uint32_t Castle::getSY() const {
    return 2;
}
uint32_t Castle::getMaxHP() const {
    return 40000;
}
Defence Castle::getDefence() const {
	return Defence::STONE;
}
Resources Castle::getCost() const {
	return Resources();
}
std::string Castle::getTextureName() const {
	return "castle";
}
std::string Castle::getSoundName() const {
	return "hooray";
}
std::wstring Castle::getDescription() const {
	return *Texts::get()->get("castle_description");
}
uint32_t Castle::getRegenerationSpeed() const {
	return this->getMaxHP() / 8;
}
std::wstring Castle::getUpperCaseReadableName() const {
	return *Texts::get()->get("castle_upper_case_readable_name");
}