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


#include "BlackKnight.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


BlackKnight::BlackKnight() = default;
BlackKnight::BlackKnight(uint32_t x, uint32_t y, uint32_t playerId) :
	WarriorNearSingleAttacker(x, y, playerId) {

}
std::shared_ptr<Warrior>  BlackKnight::cloneWarrior() const {
	return std::make_shared<BlackKnight>(*this);
}
UUID BlackKnight::getTypeUUID() const {
	return UUIDs::get().get("black_knight");
}
std::string BlackKnight::getBeenHitSoundName() const {
    return "ouch";
}
boost::optional<std::string> BlackKnight::getStartAttackSoundName() const {
    return boost::make_optional<std::string>("swing");
}
uint32_t BlackKnight::getMaxHP() const {
	return Parameters::get().getInt("black_knight_max_hp");
}
Defence BlackKnight::getBaseDefence() const {
	return Parameters::get().getDefence("black_knight_defence");
}
Damage BlackKnight::getBaseDamage() const {
    return Parameters::get().getDamage("black_knight_damage");
}
Resources BlackKnight::getCost() const {
	return Parameters::get().getResources("black_knight_cost");
}
uint32_t BlackKnight::getTimeToProduce() const {
	return Parameters::get().getInt("black_knight_time_to_produce");
}
std::string BlackKnight::getSoundName() const {
	return "breath";
}
StringLcl BlackKnight::getDescription() const {
	return StringLcl("{black_knight_description}");
}
uint32_t BlackKnight::getTalkingAnimationsNumberInSet() const {
	return 9;
}
uint32_t BlackKnight::getRunningAnimationsNumberInSet() const {
    return 8;
}
uint32_t BlackKnight::getBeenHitAnimationsNumberInSet() const {
	return 9;
}
uint32_t BlackKnight::getTippingOverAnimationsNumberInSet() const {
	return 9;
}
uint32_t BlackKnight::getAttackAnimationsNumberInSet() const {
    return 13;
}
std::string BlackKnight::getBaseTextureName() const {
	return "black_knight";
}
uint32_t BlackKnight::getMovementPoints() const {
	return Parameters::get().getInt("black_knight_movement_points");
}
uint32_t BlackKnight::getPopulation() const {
	return Parameters::get().getInt("black_knight_population");
}


BOOST_CLASS_EXPORT_IMPLEMENT(BlackKnight)