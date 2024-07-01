/*
 *  Conquesta
 *  Copyright (C) 2023 mr2anderson
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


#include "ResourceBar.hpp"


#pragma once


class Player {
public:
	Player();
	Player(uint32_t id);
	uint32_t getId() const;
	const ResourceBar *getConstResourceBarPtr() const;
	void addResource(const std::string& id, int32_t value);
	void subResource(const std::string& id, int32_t value);
	int32_t getResource(const std::string& id) const;
	friend bool operator==(const Player& a, const Player& b);
	friend bool operator!=(const Player& a, const Player& b);
private:
	uint32_t id;
	ResourceBar bar;
};