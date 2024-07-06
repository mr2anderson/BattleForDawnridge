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


Resources::Resources() = default;
Resources::Resources(const std::vector<Resource>& v) {
	for (const auto& r : v) {
		this->plus(r);
	}
}
void Resources::plus(const Resource& resource) {
	this->map[resource.type] += resource.n;
}
void Resources::minus(const Resource& resource) {
	this->map[resource.type] -= resource.n;
}
void Resources::plus(const Resources& resources) {
	for (const auto& a : resources.map) {
		this->map[a.first] += a.second;
	}
}
void Resources::minus(const Resources& resources) {
	for (const auto& a : resources.map) {
		this->map[a.first] -= a.second;
	}
}
int32_t Resources::get(const std::string& type) const {
	if (this->map.find(type) == this->map.end()) {
		return 0;
	}
	return this->map.at(type);
}
std::wstring Resources::getReadableInfo() const {
	std::wstring result;
	for (const auto& a : this->map) {
		Resource r(a.first, a.second);
		result = result + r.getReadableInfo() + L' ';
	}
	if (!result.empty()) {
		result.pop_back();
	}
	return result;
}
bool operator >=(const Resources& a, const Resources& b) {
	for (const auto& o : b.map) {
		if (a.get(o.first) < o.second) {
			return false;
		}
	}
	return true;
}