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


#include <vector>
#include <tuple>
#include "Trade.hpp"
#include "Resources.hpp"


#pragma once


class Unit;
class UpgradeableB;
class Market;
class ResourcePoint;
class GO;
class Building;


struct GEvent {
	GEvent();

	friend GEvent operator+(const GEvent &a, const GEvent& b);

	std::vector<Unit*> tryToAttack;
	std::vector<std::tuple<Market*, Trade>> tryToTrade;
	std::vector<Resource> subResource;
	std::vector<Resource> addResource;
	std::vector<Resources> subResources;
	std::vector<Resources> addResources;
	std::vector<std::tuple<const Unit*, uint32_t, uint32_t>> changeHighlight;
	std::vector<std::tuple<ResourcePoint*, uint32_t>> collect;
	std::vector<std::tuple<UpgradeableB*, Resources>> tryToUpgrade;
	std::vector<std::tuple<GO*, uint32_t>> addHp;
	std::vector<UpgradeableB*> decreaseUpgradeMovesLeft;
	std::vector<UpgradeableB*> increaseLevel;
	std::vector<Market*> decreaseCurrentTradeMovesLeft;
	std::vector<Building*> tryToBuild;
};