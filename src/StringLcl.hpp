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


#include <string>
#include <boost/serialization/access.hpp>
#include <unordered_map>
#include <SFML/Graphics/Color.hpp>
#include "ArchiveType.hpp"


#pragma once


class StringLcl {
public:
	StringLcl();
	StringLcl(const std::string& data);

	std::unordered_map<uint32_t, sf::Color> getColors() const;
    std::wstring getNoColor() const;

	std::string toRawString() const;

	StringLcl operator+(const StringLcl& b) const;
	StringLcl operator+(const std::string& b) const;
	StringLcl operator+(char c) const;

	void clear();

    static StringLcl COLOR(uint32_t r, uint32_t g, uint32_t b);
    static StringLcl COLOR(sf::Color c);
private:
	std::string data;

	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive& ar, const unsigned int version) {
		ar & this->data;
	}

    std::wstring toWstring() const;
};