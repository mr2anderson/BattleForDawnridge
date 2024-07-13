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


#include <random>
#include <unordered_map>
#include "PlayerPointerColors.hpp"



PlayerPointerColors* PlayerPointerColors::singletone = nullptr;


PlayerPointerColors::PlayerPointerColors() {
	std::random_device rd;
	std::mt19937 mersenne(rd());

	std::vector<std::string> all = { "purple", "green", "blue", "grey" };
	while (!all.empty()) {
		uint32_t index = mersenne() % all.size();
		if (index != all.size() - 1) {
			std::swap(all[index], all.back());
		}
		this->names.push_back(all.back());
		all.pop_back();
	}
}
std::string PlayerPointerColors::getColorName(uint32_t playerId) const {
	return this->names.at(playerId - 1);
}