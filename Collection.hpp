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


#pragma once


template <typename T> class Collection {
public:
	Collection() = default;

	void push(T* t) {
		this->data.push_back(t);
	}
	uint32_t size() const {
		return this->data.size();
	}
	T* at(uint32_t i) {
		return this->data.at(i);
	}
	const T* at(uint32_t i) const {
		return this->data.at(i);
	}
private:
	std::vector<T*> data;
};