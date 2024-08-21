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


#include <chrono>
#include <cstdint>
#include "ArchiveType.hpp"


#pragma once


class Clock {
public:
    Clock();

    uint32_t getMS() const;
    uint64_t getNS() const;
    float getSecondsAsFloat() const;
    void restart();
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    uint64_t nsBonus;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        if (Archive::is_saving::value) {
            this->nsBonus = this->getNS();
        }
        if (Archive::is_loading::value) {
            this->restart();
        }
        ar & this->nsBonus;
    }
};