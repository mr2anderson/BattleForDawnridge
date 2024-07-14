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


#include <map>
#include <limits>
#include <optional>
#include <tuple>
#include "FitTableElement.hpp"
#include "Move.hpp"


#pragma once


class MovementGraph {
public:
	MovementGraph(uint32_t mapW, uint32_t mapH);

	void set(uint32_t x, uint32_t y, bool canStay, uint32_t movementCost);
    Move getMove(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t movePoints);
    std::vector<std::tuple<uint32_t, uint32_t>> getMoves(uint32_t x, uint32_t y, uint32_t movePoints);
private:
	uint32_t mapW, mapH;
	std::map<std::tuple<uint32_t, uint32_t>, FitTableElement> fitTable;

    static void REVERSE_QUEUE(std::queue<std::string> &q);
	void djkstra(std::tuple<uint32_t, uint32_t> s, uint32_t movePoints, std::map<std::tuple<uint32_t, uint32_t>, uint64_t> &dist, std::map<std::tuple<uint32_t, uint32_t>, std::tuple<uint32_t, uint32_t>> &fromToStory);
};