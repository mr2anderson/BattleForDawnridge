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
#include "Balance.hpp"


Resin::Resin() = default;
Resin::Resin(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new ResinSpec());
}
Building* Resin::cloneBuilding() const {
	return new Resin(*this);
}
uint32_t Resin::getSX() const {
	return Balance::get()->getInt("resin_sx");
}
uint32_t Resin::getSY() const {
	return Balance::get()->getInt("resin_sy");
}
uint32_t Resin::getMaxHP() const {
	return Balance::get()->getInt("resin_max_hp");
}
Defence Resin::getDefence() const {
	return Balance::get()->getDefence("resin_defence");
}
Resources Resin::getCost() const {
	return Balance::get()->getResources("resin_cost");
}
uint32_t Resin::getRegenerationSpeed() const {
	return Balance::get()->getInt("resin_regeneration_speed");
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