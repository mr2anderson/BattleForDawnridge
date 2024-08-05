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
#include <string>
#include "UUID.hpp"


#pragma once


class UUIDs {
public:
    static UUIDs* get() {
        if (UUIDs::singletone == nullptr) {
            UUIDs::singletone = new UUIDs();
        }
        return UUIDs::singletone;
    }

    UUID get(const std::string& id);
private:
    UUIDs() = default;
    UUIDs(const UUIDs& copy);
    static UUIDs* singletone;

    std::unordered_map<std::string, UUID> data;
};