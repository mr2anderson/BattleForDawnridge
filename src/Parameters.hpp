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
#include "Damage.hpp"
#include "Resources.hpp"


#pragma once


class Parameters {
public:
    static Parameters& get() {
        static Parameters instance;
        return instance;
    }

    void load();
    Damage getDamage(const std::string& id) const;
    Defence getDefence(const std::string& id) const;
    Resources getResources(const std::string& id) const;
    Resource getResource(const std::string& id) const;
    uint32_t getInt(const std::string& id) const;
    double getDouble(const std::string& id) const;
private:
    Parameters() = default;
    Parameters(const Parameters& copy);

    std::unordered_map<std::string, Damage> damages;
    std::unordered_map<std::string, Defence> defences;
    std::unordered_map<std::string, Resources> resources;
    std::unordered_map<std::string, Resource> resource;
    std::unordered_map<std::string, uint32_t> ints;
    std::unordered_map<std::string, double> doubles;
};