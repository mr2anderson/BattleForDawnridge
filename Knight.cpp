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


Knight::Knight() = default;
Knight::Knight(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
	Warrior(x, y, playerId, units, go, mapW, mapH) {

}
Warrior* Knight::cloneWarrior() const {
	return new Knight(*this);
}
uint32_t Knight::getMaxHP() const {
    return 20000;
}
Damage Knight::getDamage() const {
	return { 5000, Damage::TYPE::CUT };
}
Defence Knight::getDefence() const {
	return Defence::HUMAN;
}
Resources Knight::getCost() const {
	return Resources({ Resource("food", 10000), Resource("iron", 10000) });
}
uint32_t Knight::getTimeToProduce() const {
	return 8;
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
	return 4;
}