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
#include "QuarrySpec.hpp"
#include "Texts.hpp"
#include "Balance.hpp"


Quarry::Quarry() = default;
Quarry::Quarry(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new QuarrySpec());
}
Building* Quarry::createSameTypeBuilding() const {
	return new Quarry(this->getX(), this->getY(), this->getPlayerId());
}
uint32_t Quarry::getSX() const {
	return Balance::get()->getInt("quarry_sx");
}
uint32_t Quarry::getSY() const {
	return Balance::get()->getInt("quarry_sy");
}
uint32_t Quarry::getMaxHP() const {
	return Balance::get()->getInt("quarry_max_hp");
}
Defence Quarry::getDefence() const {
	return Balance::get()->getDefence("quarry_defence");
}
Resources Quarry::getCost() const {
	return Balance::get()->getResources("quarry_cost");
}
uint32_t Quarry::getRegenerationSpeed() const {
	return Balance::get()->getInt("quarry_regeneration_speed");
}
std::string Quarry::getTextureName() const {
	return "quarry";
}
std::string Quarry::getSoundName() const {
	return "stone";
}
std::wstring Quarry::getDescription() const {
	return *Texts::get()->get("quarry_description");
}
