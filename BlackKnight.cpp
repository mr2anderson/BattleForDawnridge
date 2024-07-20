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
#include "Texts.hpp"


BlackKnight::BlackKnight() = default;
BlackKnight::BlackKnight(uint32_t x, uint32_t y, uint32_t playerId) :
	Warrior(x, y, playerId) {

}
Warrior* BlackKnight::cloneWarrior() const {
	return new BlackKnight(*this);
}
uint32_t BlackKnight::getMaxHP() const {
    return 7500;
}
Damage BlackKnight::getDamage() const {
	return Damage(5000, Damage::TYPE::CUT);
}
Defence BlackKnight::getDefence() const {
	return Defence::HUMAN;
}
Resources BlackKnight::getCost() const {
	return Resources({ Resource("food", 12000), Resource("iron", 12000)});
}
uint32_t BlackKnight::getTimeToProduce() const {
	return 9;
}
std::string BlackKnight::getSoundName() const {
	return "breath";
}
std::wstring BlackKnight::getDescription() const {
	return *Texts::get()->get("black_knight_description");
}
uint32_t BlackKnight::getTalkingAnimationsNumberInSet() const {
	return 9;
}
uint32_t BlackKnight::getRunningAnimationsNumberInSet() const {
	return 8;
}
uint32_t BlackKnight::getAttackAnimationsNumberInSet() const {
	return 13;
}
uint32_t BlackKnight::getBeenHitAnimationsNumberInSet() const {
	return 9;
}
uint32_t BlackKnight::getTippingOverAnimationsNumberInSet() const {
	return 9;
}
std::string BlackKnight::getBaseTextureName() const {
	return "black_knight";
}
uint32_t BlackKnight::getMovementPoints() const {
	return 4;
}
uint32_t BlackKnight::getPopulation() const {
	return 1;
}