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


#include "Infantryman.hpp"
#include "Texts.hpp"


Infantryman::Infantryman() = default;
Infantryman::Infantryman(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
	Warrior(x, y, playerId, units, go, mapW, mapH) {

}
Warrior* Infantryman::cloneWarrior() const {
	return new Infantryman(*this);
}
uint32_t Infantryman::getMaxHP() const {
    return 10000;
}
Damage Infantryman::getDamage() const {
	return { 2500, Damage::TYPE::CUT };
}
Defence Infantryman::getDefence() const {
	return DEFENCE::HUMAN;
}
Resources Infantryman::getCost() const {
	return Resources({ Resource("food", 5000 )});
}
uint32_t Infantryman::getTimeToProduce() const {
	return 3;
}
std::string Infantryman::getSoundName() const {
	return "hooray";
}
std::wstring Infantryman::getDescription() const {
	return *Texts::get()->get("infantryman_description");
}
uint32_t Infantryman::getTalkingAnimationsNumberInSet() const {
	return 7;
}
uint32_t Infantryman::getRunningAnimationsNumberInSet() const {
	return 8;
}
uint32_t Infantryman::getAttackAnimationsNumberInSet() const {
	return 13;
}
uint32_t Infantryman::getBeenHitAnimationsNumberInSet() const {
	return 7;
}
uint32_t Infantryman::getTippingOverAnimationsNumberInSet() const {
	return 9;
}
std::string Infantryman::getBaseTextureName() const {
	return "infantryman";
}
uint32_t Infantryman::getMovementPoints() const {
	return 6;
}