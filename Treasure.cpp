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


#include "Treasure.hpp"
#include "Texts.hpp"


Treasure::Treasure() = default;
Treasure::Treasure(uint32_t x, uint32_t y) : ResourcePoint(x, y) {

}
Events Treasure::newMove(uint32_t playerId) {
	this->alreadyCollected = false;
	return Events();
}
Events Treasure::tryToCollect(uint32_t playerId, uint32_t value) {
	if (!this->alreadyCollected) {
		this->alreadyCollected = true;
		return this->ResourcePoint::tryToCollect(playerId, value);
	}
	return Events();
}
uint32_t Treasure::getSX() const {
    return 1;
}
uint32_t Treasure::getSY() const {
    return 1;
}
uint32_t Treasure::getMaxHP() const {
    return 50000;
}
std::string Treasure::getResourceType() const {
	return "gold";
}
std::string Treasure::getSoundName() const {
	return "gold";
}
std::string Treasure::getTextureName() const {
	return "treasure";
}
std::wstring Treasure::getDescription() const {
	return *Texts::get()->get("treasure_description");
}