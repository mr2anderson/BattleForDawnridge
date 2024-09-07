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


#include "Golem.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Golem::Golem() = default;
Golem::Golem(uint32_t x, uint32_t y, uint32_t playerId) :
	WarriorNearSingleAttacker(x, y, playerId) {

}
std::shared_ptr<Warrior> Golem::cloneWarrior() const {
	return std::make_shared<Golem>(*this);
}
UUID Golem::getTypeUUID() const {
	return UUIDs::get().get("golem");
}
std::string Golem::getBeenHitSoundName() const {
	return "roar";
}
boost::optional<std::string> Golem::getStartAttackSoundName() const {
	return boost::make_optional<std::string>("roar");
}
uint32_t Golem::getMaxHP() const {
	return Parameters::get().getInt("golem_max_hp");
}
Defence Golem::getBaseDefence() const {
	return Parameters::get().getDefence("golem_defence");
}
Damage Golem::getBaseDamage() const {
	return Parameters::get().getDamage("golem_damage");
}
Resources Golem::getCost() const {
	return Parameters::get().getResources("golem_cost");
}
uint32_t Golem::getTimeToProduce() const {
	return Parameters::get().getInt("golem_time_to_produce");
}
std::string Golem::getSoundName() const {
	return "roar";
}
StringLcl Golem::getDescription() const {
	return StringLcl("{golem_description}");
}
uint32_t Golem::getTalkingAnimationsNumberInSet() const {
	return 9;
}
uint32_t Golem::getRunningAnimationsNumberInSet() const {
	return 8;
}
uint32_t Golem::getBeenHitAnimationsNumberInSet() const {
	return 9;
}
uint32_t Golem::getTippingOverAnimationsNumberInSet() const {
	return 17;
}
uint32_t Golem::getAttackAnimationsNumberInSet() const {
	return 13;
}
std::string Golem::getBaseTextureName() const {
	return "golem";
}
uint32_t Golem::getMovementPoints() const {
	return Parameters::get().getInt("golem_movement_points");
}
uint32_t Golem::getPopulation() const {
	return Parameters::get().getInt("golem_population");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Golem)