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


#include <unordered_map>
#include "Resource.hpp"


#pragma once


class Resources {
public:
	Resources();
	Resources(const std::vector<Resource>& v);

	void plus(const Resource& resource);
	void plus(const Resource& resource, uint32_t limit);
	void minus(const Resource& resource);
	void plus(const Resources& resources);
	void plus(const Resources& resources, const Resources& limit);
	void minus(const Resources& resources);
	void limit(const Resources& resources);
	int32_t get(const std::string& id) const;
	std::wstring getReadableInfo() const;
	friend bool operator>=(const Resources& a, const Resources& b);
private:
	std::unordered_map<std::string, int32_t> map;
};