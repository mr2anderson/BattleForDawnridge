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


#include <cassert>
#include "Building.hpp"
#include "Trade.hpp"
#include "SelectWindow.hpp"


#pragma once


class Caravan : public Building {
public:
	Caravan(uint32_t x, uint32_t y, const Player *playerPtr);
	GameObjectResponse doTrade(const Trade& trade);
	GameObjectResponse newMove(const Player& currentPlayer, uint32_t windowW, uint32_t windowH);
private:
	Trade currentTrade;

	std::string getTextureName() const override;
	bool busy() const;
	GameObjectResponse getGameObjectResponse(const Player& player, uint32_t windowW, uint32_t windowH) override;
};