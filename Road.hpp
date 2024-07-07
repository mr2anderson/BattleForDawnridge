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


#include "TerritoryConductorB.hpp"
#include "GameActionWindow.hpp"


#pragma once


class Road : public TerritoryConductorB {
public:
	Road();
	Road(uint32_t x, uint32_t y, Player* playerPtr, std::vector<TerritoryOriginB*> *originsPtr, std::vector<TerritoryConductorB*> *conductorsPtr);
	Building* clone() const override;

	Event newMove(Player* player) override;
	Resources getCost() const override;
	std::string getTextureName() const override;
	std::string getSoundName() const override;
	std::wstring getDescription() const override;
private:
	uint32_t getRegenerationSpeed() const override;
	std::wstring getUpperCaseReadableName() const override;
	uint32_t getRadius() const override;
	Event getSelectionW();
	Event getGameObjectResponse(Player* player) override;
};