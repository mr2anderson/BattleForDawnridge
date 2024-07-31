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


#include <set>
#include <vector>
#include <cstdint>
#include "TypeComp.hpp"


#pragma once


template <typename T> class Collection {
public:
	Collection() = default;

	void push(T* t) {
		this->data.insert(t);
        this->reindex();
	}
	uint32_t size() const {
		return this->data.size();
	}
	T* at(uint32_t i) {
        return this->dataIndexed[i];
	}
	const T* at(uint32_t i) const {
        return this->dataIndexed[i];
	}
private:
    friend class Collections;

	std::multiset<T*, TypeComp<T>> data;
    std::vector<T*> dataIndexed;

    void reindex() {
        this->dataIndexed.clear();
        this->dataIndexed.reserve(this->data.size());
        for (auto &a : this->data) {
            this->dataIndexed.push_back(a);
        }
    }
};