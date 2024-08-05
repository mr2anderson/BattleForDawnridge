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


#include "Barracks.hpp"
#include "BarracksSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Barracks::Barracks() = default;
Barracks::Barracks(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new BarracksSpec());
}
Building* Barracks::createSameTypeBuilding() const {
	return new Barracks(this->getX(), this->getY(), this->getPlayerId());
}
UUID Barracks::getTypeUUID() const {
	return UUIDs::get()->get("barracks");
}
uint32_t Barracks::getSX() const {
	return Parameters::get()->getInt("barracks_sx");
}
uint32_t Barracks::getSY() const {
	return Parameters::get()->getInt("barracks_sy");
}
uint32_t Barracks::getMaxHP() const {
	return Parameters::get()->getInt("barracks_max_hp");
}
Defence Barracks::getDefence() const {
	return Parameters::get()->getDefence("barracks_defence");
}
Resources Barracks::getCost() const {
	return Parameters::get()->getResources("barracks_cost");
}
std::string Barracks::getTextureName() const {
	return "barracks";
}
std::string Barracks::getSoundName() const {
	return "hooray";
}
std::wstring Barracks::getDescription() const {
	return *Locales::get()->get("barracks_description");
}
uint32_t Barracks::getRegenerationSpeed() const {
	return Parameters::get()->getInt("barracks_regeneration_speed");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Barracks)