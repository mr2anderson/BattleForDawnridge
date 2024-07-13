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
#include "Texts.hpp"


Legioner::Legioner() = default;
Legioner::Legioner(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
	Warrior(x, y, 1000, playerId, units, go, mapW, mapH) {

}
Warrior* Legioner::cloneWarrior() const {
	return new Legioner(*this);
}
Resources Legioner::getCost() const {
	return Resources({ Resource("food", 5000), Resource("iron", 2500)});
}
uint32_t Legioner::getTimeToProduce() const {
	return 3;
}
std::string Legioner::getSoundName() const {
	return "sword";
}
std::wstring Legioner::getDescription() const {
	return *Texts::get()->get("legioner_description");
}
uint32_t Legioner::getTalkingAnimationsNumberInSet() const {
	return 8;
}
uint32_t Legioner::getWalkingAnimationsNumberInSet() const {
	return 8;
}
uint32_t Legioner::getAttackAnimationsNumberInSet() const {
	return 12;
}
uint32_t Legioner::getBeenHitAnimationsNumberInSet() const {
	return 8;
}
uint32_t Legioner::getTippingOverAnimationsNumberInSet() const {
	return 12;
}
std::string Legioner::getBaseTextureName() const {
	return "legioner";
}
uint32_t Legioner::getMovementPoints() const {
	return 6;
}