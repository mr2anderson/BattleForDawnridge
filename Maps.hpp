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


#include <fstream>
#include "Map.hpp"
#include "PlainsGeneration.hpp"


#pragma once


class Maps {
public:
    static Maps* get() {
        if (Maps::singletone == nullptr) {
            Maps::singletone = new Maps();
        }
        return Maps::singletone;
    }

    void add(const std::string &name, const std::string& path);
    std::shared_ptr<Map> load(const std::string& name);

    static const uint32_t THUMBNAIL_SIZE;
private:
    Maps() = default;
    Maps(const Maps& copy);
    static Maps* singletone;

    std::unordered_map<std::string, std::string> paths;

    void generateThumbnail(const std::string& name);
};