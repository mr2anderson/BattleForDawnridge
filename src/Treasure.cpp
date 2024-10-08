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
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"
#include "CircleLightSourceSin.hpp"


Treasure::Treasure() = default;
Treasure::Treasure(uint32_t x, uint32_t y) : ConductionResourcePoint(x, y) {
}
UUID Treasure::getTypeUUID() const {
	return UUIDs::get().get("treasure");
}
uint32_t Treasure::getSX() const {
	return Parameters::get().getInt("treasure_sx");
}
uint32_t Treasure::getSY() const {
	return Parameters::get().getInt("treasure_sy");
}
uint32_t Treasure::getMaxHP() const {
	return Parameters::get().getInt("treasure_max_hp");
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
StringLcl Treasure::getDescription() const {
	return StringLcl("{treasure_description}");
}
std::shared_ptr<ILightSource> Treasure::getLightSource() const {
	return std::make_shared<CircleLightSourceSin>(this->getCenterX(), this->getCenterY(), 28.f, 0.2f, 1.5f);
}


BOOST_CLASS_EXPORT_IMPLEMENT(Treasure)