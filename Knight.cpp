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


#include "Knight.hpp"
#include "Texts.hpp"
#include "Balance.hpp"


Knight::Knight() = default;
Knight::Knight(uint32_t x, uint32_t y, uint32_t playerId) :
	Warrior(x, y, playerId) {

}
Warrior* Knight::cloneWarrior() const {
	return new Knight(*this);
}
uint8_t Knight::getType() const {
    return Warrior::TYPE::HUMAN;
}
uint32_t Knight::getMaxHP() const {
	return Balance::get()->getInt("knight_max_hp");
}
Damage Knight::getBaseDamage() const {
	return Balance::get()->getDamage("knight_damage");
}
Defence Knight::getBaseDefence() const {
	return Balance::get()->getDefence("knight_defence");
}
Resources Knight::getCost() const {
	return Balance::get()->getResources("knight_cost");
}
uint32_t Knight::getTimeToProduce() const {
	return Balance::get()->getInt("knight_time_to_produce");
}
std::string Knight::getSoundName() const {
	return "knight";
}
std::wstring Knight::getDescription() const {
	return *Texts::get()->get("knight_description");
}
uint32_t Knight::getTalkingAnimationsNumberInSet() const {
	return 12;
}
uint32_t Knight::getRunningAnimationsNumberInSet() const {
	return 12;
}
uint32_t Knight::getAttackAnimationsNumberInSet() const {
	return 12;
}
uint32_t Knight::getBeenHitAnimationsNumberInSet() const {
	return 12;
}
uint32_t Knight::getTippingOverAnimationsNumberInSet() const {
	return 12;
}
std::string Knight::getBaseTextureName() const {
	return "knight";
}
uint32_t Knight::getMovementPoints() const {
	return Balance::get()->getInt("knight_movement_points");
}
uint32_t Knight::getPopulation() const {
	return Balance::get()->getInt("knight_population");
}