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


#include "HpSensitiveB.hpp"
#include "Trade.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "SubResourceEvent.hpp"
#include "AddResourceEvent.hpp"
#include "DecreaseCurrentTradeMovesLeftEvent.hpp"
#include "TryToTradeEvent.hpp"


#pragma once


class Market : public HpSensitiveB {
public:
	Market();
	Market(uint32_t x, uint32_t y, uint32_t playerId);
	Building* cloneBuilding() const override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Events doTrade(const Trade& trade);
	void decreaseCurrentTradeMovesLeft();
	Events newMove(uint32_t playerId);
	Resources getCost() const override;
	std::string getTextureName() const override;
	std::string getSoundName() const override;
	std::wstring getDescription() const override;
private:
	Trade currentTrade;

	HorizontalSelectionWindowComponent getTradeComponent(std::shared_ptr<TryToTradeEvent> gameEventTrade) const;
	HorizontalSelectionWindowComponent getBusyWithTradeComponent() const;
	uint32_t getRegenerationSpeed() const override;
	std::wstring getUpperCaseReadableName() const override;
	bool busy() const;
	uint32_t getTradeStartTime() const;
	void drawCurrentTradeShortInfo(sf::RenderTarget& target, sf::RenderStates states) const;
	Events handleCurrentTrade();
	Events getSelectionW();
	Events getGameObjectResponse(uint32_t playerId) override;
};