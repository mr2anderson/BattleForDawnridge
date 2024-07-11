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
#include "Texts.hpp"


Market::Market() = default;
Market::Market(uint32_t x, uint32_t y, uint32_t playerId) : 
	TradingB(x, y, 3, 3, 15000, playerId),
	Building(x, y, 3, 3, 15000, playerId) {

}
Building* Market::cloneBuilding() const {
	return new Market(*this);
}
Resources Market::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 30000));
	return cost;
}
std::string Market::getTextureName() const {
	return "market";
}
std::string Market::getSoundName() const {
	return "gold";
}
std::wstring Market::getDescription() const {
	return *Texts::get()->get("market_description");
}
uint32_t Market::getRegenerationSpeed() const {
	return 10000;
}
std::wstring Market::getUpperCaseReadableName() const {
	return *Texts::get()->get("market_upper_case_readable_name");
}
std::vector<Trade> Market::getTrades() const {
	const uint32_t time = 5;
	const uint32_t goldVolume = 5000;

	std::vector<Trade> trades;

	trades.emplace_back(
		Resource("gold", goldVolume),
		Resource("food", 3 * goldVolume),
		time
	);
	trades.emplace_back(
		Resource("gold", goldVolume),
		Resource("wood", 3 * goldVolume),
		time
	);
	trades.emplace_back(
		Resource("gold", goldVolume),
		Resource("stone", 3 * goldVolume),
		time
	);
	trades.emplace_back(
		Resource("gold", goldVolume),
		Resource("iron", goldVolume),
		time
	);

	return trades;
}