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


#include "Arable.hpp"
#include "ArableSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"


Arable::Arable() = default;
Arable::Arable(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new ArableSpec());
}
Building* Arable::createSameTypeBuilding() const {
	return new Arable(this->getX(), this->getY(), this->getPlayerId());
}
uint32_t Arable::getSX() const {
	return Parameters::get()->getInt("arable_sx");
}
uint32_t Arable::getSY() const {
	return Parameters::get()->getInt("arable_sy");
}
uint32_t Arable::getMaxHP() const {
	return Parameters::get()->getInt("arable_max_hp");
}
Defence Arable::getDefence() const {
	return Parameters::get()->getDefence("arable_defence");
}
Resources Arable::getCost() const {
	return Parameters::get()->getResources("arable_cost");
}
std::string Arable::getTextureName() const {
	return "arable";
}
std::string Arable::getSoundName() const {
	return "food";
}
std::wstring Arable::getDescription() const {
	return *Locales::get()->get("arable_description");
}
uint32_t Arable::getRegenerationSpeed() const {
	return Parameters::get()->getInt("arable_regeneration_speed");
}
