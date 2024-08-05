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


#include "Stone.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDS.hpp"


Stone::Stone() = default;
Stone::Stone(uint32_t x, uint32_t y) : ResourcePoint(x, y) {

}
UUID Stone::getTypeUUID() const {
	return UUIDs::get()->get("stone");
}
uint32_t Stone::getSX() const {
	return Parameters::get()->getInt("stone_sx");
}
uint32_t Stone::getSY() const {
	return Parameters::get()->getInt("stone_sy");
}
uint32_t Stone::getMaxHP() const {
	return Parameters::get()->getInt("stone_max_hp");
}
std::string Stone::getResourceType() const {
	return "stone";
}
std::string Stone::getSoundName() const {
	return "stone";
}
std::string Stone::getTextureName() const {
	return "stone";
}
std::wstring Stone::getDescription() const {
	return *Locales::get()->get("stone_description");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Stone)