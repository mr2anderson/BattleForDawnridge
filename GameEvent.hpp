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


#pragma once


class Unit;
class Caravan;
class ResourcePoint;


struct GameEvent {
	GameEvent();
	std::vector<Unit*> tryToAttack;
	std::vector<std::tuple<Caravan*, Trade>> tryToTrade;
	std::vector<Resource> startTrade;
	std::vector<Resource> finishTrade;
	std::vector<std::tuple<const Unit*, uint32_t, uint32_t>> changeHighlight;
	std::vector<std::tuple<ResourcePoint*, uint32_t>> collect;
};