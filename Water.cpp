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


#include "Water.hpp"
#include "Texts.hpp"


const uint32_t Water::TOTAL_ANIMATIONS = 40;


Water::Water() = default;
Water::Water(uint32_t x, uint32_t y) :
	ImpassableObstacle(x, y) {

}
uint32_t Water::getSY() const {
    return 1;
}
uint32_t Water::getSX() const {
    return 1;
}
std::string Water::getTextureName() const {
	return "water" + std::to_string(this->clock.getElapsedTime().asMilliseconds() / (2 * 1000 / TOTAL_ANIMATIONS) % TOTAL_ANIMATIONS);
}
std::string Water::getSoundName() const {
	return "water";
}
std::wstring Water::getDescription() const {
	return *Texts::get()->get("water_description");
}
bool Water::isLowObstacle() const {
    return true;
}