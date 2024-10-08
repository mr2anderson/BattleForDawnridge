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
#include <boost/serialization/vector.hpp>
#include "Player.hpp"
#include "ArchiveType.hpp"


#pragma once


class Players {
public:
	Players();

	bool hasError() const {
		for (uint32_t i = 0; i < this->v.size(); i = i + 1) {
			if (i + 1 != this->v.at(i).getId()) {
				return true;
			}
		}
		return false;
	}

	void addPlayer(const Player& player);
	Player* getPlayerPtr(uint32_t id);
	uint32_t total() const;
private:
	std::vector<Player> v;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->v;
    }
};