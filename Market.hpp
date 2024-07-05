/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "UpgradeableB.hpp"
#include "HpSensitiveB.hpp"
#include "Trade.hpp"
#include "SelectionW.hpp"


#pragma once


class Market : public UpgradeableB, public HpSensitiveB {
public:
	Market();
	Market(uint32_t x, uint32_t y, const Player *playerPtr);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Event doTrade(const Trade& trade);
	void decreaseCurrentTradeMovesLeft();
	Event newMove(const Player& currentPlayer);
	bool works() const override;
	Resources getCost() const override;
	std::string getTextureName() const override;
	std::string getSoundName() const override;
	std::wstring getDescription() const override;
private:
	Trade currentTrade;

	uint32_t getRegenerationSpeed() const override;
	Resources getUpgradeCost() const override;
	uint32_t getUpgradeTime() const override;
	std::wstring getReadableName() const override;
	bool busy() const;
	static uint32_t GET_TRADE_START_TIME(uint32_t level);
	uint32_t getTradeStartTime() const;
	void drawCurrentTradeShortInfo(sf::RenderTarget& target, sf::RenderStates states) const;
	Event handleCurrentTrade();
	Event getSelectionW();
	void addTrade(std::vector<SelectionWComponent>& components, const Event& gameEventTrade);
	Event handleBusyWithTrade() const;
	Event getGameObjectResponse(const Player& player) override;
};