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


#include "Infirmary.hpp"
#include "InfirmarySpec.hpp"
#include "Texts.hpp"
#include "Balance.hpp"


Infirmary::Infirmary() = default;
Infirmary::Infirmary(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new InfirmarySpec());
}
Building* Infirmary::createSameTypeBuilding() const {
	return new Infirmary(this->getX(), this->getY(), this->getPlayerId());
}
uint32_t Infirmary::getSX() const {
	return Balance::get()->getInt("infirmary_sx");
}
uint32_t Infirmary::getSY() const {
	return Balance::get()->getInt("infirmary_sy");
}
uint32_t Infirmary::getMaxHP() const {
	return Balance::get()->getInt("infirmary_max_hp");
}
Defence Infirmary::getDefence() const {
	return Balance::get()->getDefence("infirmary_defence");
}
Resources Infirmary::getCost() const {
	return Balance::get()->getResources("infirmary_cost");
}
std::string Infirmary::getTextureName() const {
	return "infirmary";
}
std::string Infirmary::getSoundName() const {
	return "hooray";
}
std::wstring Infirmary::getDescription() const {
	return *Texts::get()->get("infirmary_description");
}
uint32_t Infirmary::getRegenerationSpeed() const {
	return Balance::get()->getInt("infirmary_regeneration_speed");
}