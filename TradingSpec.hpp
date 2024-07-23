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


#include "IBuildingSpec.hpp"
#include "Trade.hpp"


#pragma once


class TradingSpec : public IBuildingSpec {
public:
	TradingSpec();

	Events doTrade(const Building *b, const Trade& trade);
	void decreaseCurrentTradeMovesLeft();
	Events getActiveNewMoveEvent(const Building *b, MapState* state) override;
	std::vector<HorizontalSelectionWindowComponent> getComponents(const Building *b, MapState* state) override;
	std::optional<BuildingShortInfo> getShortInfo(const Building *b) const override;
	virtual std::vector<Trade> getTrades() const = 0;
private:
	Trade currentTrade;

	bool busy() const;
	HorizontalSelectionWindowComponent getTradeComponent(const Building* b, const Resources& playerResources, const Trade& trade);
};