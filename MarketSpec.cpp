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


#include "MarketSpec.hpp"


MarketSpec::MarketSpec() = default;
IBuildingSpec* MarketSpec::clone() const {
	return new MarketSpec(*this);
}
std::vector<Trade> MarketSpec::getTrades() const {
	const uint32_t time = 4;
	const uint32_t goldVolume = 5000;

	std::vector<Trade> trades;

	trades.emplace_back(
		Resource("gold", goldVolume),
		Resource("food", 2 * goldVolume),
		time
	);
	trades.emplace_back(
		Resource("gold", goldVolume),
		Resource("wood", 2 * goldVolume),
		time
	);
	trades.emplace_back(
		Resource("gold", goldVolume),
		Resource("stone", 2 * goldVolume),
		time
	);
	trades.emplace_back(
		Resource("gold", goldVolume),
		Resource("iron", goldVolume),
		time
	);

	return trades;
}