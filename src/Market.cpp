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


#include "Market.hpp"
#include "MarketSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Market::Market() = default;
Market::Market(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new MarketSpec());
}
Building* Market::createSameTypeBuilding() const {
	return new Market(this->getX(), this->getY(), this->getPlayerId());
}
UUID Market::getTypeUUID() const {
	return UUIDs::get()->get("market");
}
uint32_t Market::getSX() const {
	return Parameters::get()->getInt("market_sx");
}
uint32_t Market::getSY() const {
	return Parameters::get()->getInt("market_sy");
}
uint32_t Market::getMaxHP() const {
	return Parameters::get()->getInt("market_max_hp");
}
Defence Market::getDefence() const {
	return Parameters::get()->getDefence("market_defence");
}
Resources Market::getCost() const {
	return Parameters::get()->getResources("market_cost");
}
std::string Market::getTextureName() const {
	return "market";
}
std::string Market::getSoundName() const {
	return "gold";
}
StringLcl Market::getDescription() const {
	return StringLcl("{market_description}");
}
uint32_t Market::getRegenerationSpeed() const {
	return Parameters::get()->getInt("market_regeneration_speed");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Market)