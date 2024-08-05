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


#include "Legioner.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Legioner::Legioner() = default;
Legioner::Legioner(uint32_t x, uint32_t y, uint32_t playerId) :
	WarriorNearSingleAttacker(x, y, playerId) {

}
Warrior* Legioner::cloneWarrior() const {
	return new Legioner(*this);
}
UUID Legioner::getTypeUUID() const {
	return UUIDs::get()->get("legioner");
}
std::string Legioner::getBeenHitSoundName() const {
    return "ouch";
}
std::string Legioner::getStartAttackSoundName() const {
    return "swing";
}
uint32_t Legioner::getMaxHP() const {
	return Parameters::get()->getInt("legioner_max_hp");
}
Defence Legioner::getBaseDefence() const {
	return Parameters::get()->getDefence("legioner_defence");
}
Damage Legioner::getBaseDamage() const {
    return Parameters::get()->getDamage("legioner_damage");
}
Resources Legioner::getCost() const {
	return Parameters::get()->getResources("legioner_cost");
}
uint32_t Legioner::getTimeToProduce() const {
	return Parameters::get()->getInt("legioner_time_to_produce");
}
std::string Legioner::getSoundName() const {
	return "sword";
}
std::wstring Legioner::getDescription() const {
	return *Locales::get()->get("legioner_description");
}
uint32_t Legioner::getTalkingAnimationsNumberInSet() const {
	return 8;
}
uint32_t Legioner::getRunningAnimationsNumberInSet() const {
	return 8;
}
uint32_t Legioner::getBeenHitAnimationsNumberInSet() const {
	return 8;
}
uint32_t Legioner::getTippingOverAnimationsNumberInSet() const {
	return 12;
}
uint32_t Legioner::getAttackAnimationsNumberInSet() const {
    return 12;
}
std::string Legioner::getBaseTextureName() const {
	return "legioner";
}
uint32_t Legioner::getMovementPoints() const {
	return Parameters::get()->getInt("legioner_movement_points");
}
uint32_t Legioner::getPopulation() const {
	return Parameters::get()->getInt("legioner_population");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Legioner)