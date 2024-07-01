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


#include <optional>
#include <tuple>
#include "Trade.hpp"


#pragma once


class Unit;
class Caravan;


struct GameEvent {
	GameEvent();
	std::optional<Unit*> tryToAttack;
	std::optional<std::tuple<Caravan*, Trade>> tryToTrade;
	std::optional<Resource> startTrade;
	std::optional<Resource> finishTrade;
};