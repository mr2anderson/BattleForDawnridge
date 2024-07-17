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
#include "Textures.hpp"
#include "TilesetHandler.hpp"


Water::Water() = default;
Water::Water(uint32_t x, uint32_t y, uint32_t type) :
	ImpassableObstacle(x, y) {
	this->type = type;
}
uint32_t Water::getSY() const {
    return 1;
}
uint32_t Water::getSX() const {
    return 1;
}
std::string Water::getTextureName() const {
	return "water";
}
sf::IntRect Water::getTextureRect() const {
	return TilesetHandler::get()->getTextureRect(64 * this->getSX(), 64 * this->getSY(), Textures::get()->get(this->getTextureName())->getSize().x, this->type);
}
std::string Water::getSoundName() const {
	return "water";
}
std::wstring Water::getDescription() const {
	return *Texts::get()->get("water_description");
}
bool Water::isLowObstacle(uint32_t playerId) const {
    return true;
}