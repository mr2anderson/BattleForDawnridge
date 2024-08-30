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


#include "Resources.hpp"


#pragma once


class Player {
public:
	Player();
	Player(uint32_t id);

	uint32_t getId() const;
	friend bool operator==(const Player &a, const Player &b);
	friend bool operator!=(const Player &a, const Player &b);
    friend bool operator<(const Player &a, const Player &b);
	void addResource(const Resource& resource, uint32_t limit);
	void subResource(const Resource &resource);
	void addResources(const Resources& resources, const Resources& limit);
	void subResources(const Resources& resources);
	void limitResources(const Resources& limit);
	int32_t getResource(const std::string& id) const;
	Resources getResources() const;

    enum COLOR {
        BLUE = 1,
        GREEN = 2,
        PURPLE = 3
    };

    static bool IS_POSITIVE(uint32_t id);
    static bool IS_NEGATIVE(uint32_t id);
private:
	uint32_t id;
	Resources resources;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->id;
        ar & this->resources;
    }
};