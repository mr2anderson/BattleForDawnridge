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


#include "Resin.hpp"
#include "ResinSpec.hpp"
#include "Texts.hpp"


Resin::Resin() = default;
Resin::Resin(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new ResinSpec());
}
Building* Resin::cloneBuilding() const {
	return new Resin(*this);
}
uint32_t Resin::getSX() const {
	return 2;
}
uint32_t Resin::getSY() const {
	return 2;
}
uint32_t Resin::getMaxHP() const {
	return 15000;
}
Defence Resin::getDefence() const {
	return Defence::STONE;
}
Resources Resin::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 7500));
	return cost;
}
uint32_t Resin::getRegenerationSpeed() const {
	return this->getMaxHP() / 5;
}
std::string Resin::getTextureName() const {
	return "resin";
}
std::string Resin::getSoundName() const {
	return "gurgle";
}
std::wstring Resin::getDescription() const {
	return *Texts::get()->get("resin_description");
}
std::wstring Resin::getUpperCaseReadableName() const {
	return *Texts::get()->get("resin_upper_case_readable_name");
}