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


#include <cstdint>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include "StringLcl.hpp"


#pragma once


typedef boost::multiprecision::uint128_t uint128_t;


class UUID {
public:
    UUID();

    bool operator==(const UUID &b) const;
    bool operator!=(const UUID &b) const;
    bool operator<(const UUID &b) const;
    bool operator>(const UUID &b) const;
    std::size_t operator()() const;
    StringLcl toString() const;
    uint128_t value() const;
private:
    uint128_t a;
};


template <> struct std::hash<UUID> {
    std::size_t operator()(const UUID& k) const {
        return k.value().convert_to<std::size_t>();
    }
};


static const UUID INVALID_UUID = UUID();