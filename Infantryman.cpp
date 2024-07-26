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
#include "Locales.hpp"
#include "Parameters.hpp"


Infantryman::Infantryman() = default;
Infantryman::Infantryman(uint32_t x, uint32_t y, uint32_t playerId) :
	Warrior(x, y, playerId) {

}
Warrior* Infantryman::cloneWarrior() const {
	return new Infantryman(*this);
}
std::string Infantryman::getBeenHitSoundName() const {
    return "ouch";
}
uint32_t Infantryman::getMaxHP() const {
	return Parameters::get()->getInt("infantryman_max_hp");
}
Defence Infantryman::getBaseDefence() const {
	return Parameters::get()->getDefence("infantryman_defence");
}
Resources Infantryman::getCost() const {
	return Parameters::get()->getResources("infantryman_cost");
}
uint32_t Infantryman::getTimeToProduce() const {
	return Parameters::get()->getInt("infantryman_time_to_produce");
}
std::string Infantryman::getSoundName() const {
	return "infantryman";
}
std::wstring Infantryman::getDescription() const {
	return *Locales::get()->get("infantryman_description");
}
uint32_t Infantryman::getTalkingAnimationsNumberInSet() const {
	return 7;
}
uint32_t Infantryman::getRunningAnimationsNumberInSet() const {
	return 8;
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
	return Parameters::get()->getInt("infantryman_movement_points");
}
uint32_t Infantryman::getPopulation() const {
	return Parameters::get()->getInt("infantryman_population");
}