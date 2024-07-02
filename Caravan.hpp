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


#include "HpSensitiveBuilding.hpp"
#include "Trade.hpp"
#include "SelectWindow.hpp"


#pragma once


class Caravan : public HpSensitiveBuilding {
public:
	Caravan(uint32_t x, uint32_t y, const Player *playerPtr);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	GameObjectResponse doTrade(const Trade& trade);
	GameObjectResponse newMove(const Player& currentPlayer, uint32_t windowW, uint32_t windowH);
private:
	Trade currentTrade;

	GameObjectResponse currentTradeNewMove();
	bool busy() const;
	GameObjectResponse getSelectWindow();
	void drawCurrentTradeShortInfo(sf::RenderTarget& target, sf::RenderStates states) const;

	uint32_t getRegenerationSpeed() const override;
	std::string getNewWindowSoundName() const override;
	std::wstring getBuildingFinishedStr() const override;
	std::wstring getIsNotBuiltYetStr() const override;
	std::string getTextureName() const override;

	GameObjectResponse getGameObjectResponse(const Player& player, uint32_t windowW, uint32_t windowH) override;
};