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


#include <vector>
#include "PlayerColors.hpp"
#include "GlobalRandomGenerator.hpp"



PlayerColors* PlayerColors::singletone = nullptr;


PlayerColors::PlayerColors() {
	std::vector<uint32_t> all;
    for (uint32_t i = 1; i <= 3; i = i + 1) {
        all.push_back(i);
    }
	while (!all.empty()) {
		uint32_t it = GlobalRandomGenerator::get()->gen() % all.size();
		if (it != all.size() - 1) {
			std::swap(all[it], all.back());
		}
		this->indexes.push_back(all.back());
		all.pop_back();
	}
}
uint32_t PlayerColors::getBlueIndex() const {
    return this->indexes[0];
}
uint32_t PlayerColors::getGreenIndex() const {
    return this->indexes[1];
}
uint32_t PlayerColors::getPurpleIndex() const {
    return this->indexes[2];
}