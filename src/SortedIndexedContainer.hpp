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


#include <boost/serialization/access.hpp>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <boost/serialization/vector.hpp>
#include "ArchiveType.hpp"


#pragma once


// Data structure
// Sorted order
// Access O(1)
// Insert O(n)
// Memory O(n)
template<typename T, typename Comp> class SortedIndexedContainer {
public:
    SortedIndexedContainer() = default;

    void insert(T t) {
        this->vector.insert(std::upper_bound(this->vector.begin(), this->vector.end(), t, Comp()), t);
    }
    void clear() {
        this->vector.clear();
    }
    uint32_t size() const {
        return this->vector.size();
    }
    T at(uint32_t i) {
        return this->vector.at(i);
    }
    T at(uint32_t i) const {
        return this->vector.at(i);
    }
private:
    std::vector<T> vector;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->vector;
    }
};