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


#include <iostream>
#include "Maps.hpp"
#include "GO.hpp"


Maps* Maps::singletone = nullptr;


void Maps::add(const std::string &name, const std::string &path) {
    this->paths[name] = path;
    this->verify(name);
}
std::shared_ptr<Map> Maps::load(const std::string& name) {
    auto it = this->paths.find(name);
    if (it == this->paths.end()) {
        std::cerr << "Invalid map id: " << name << std::endl;
    }
    std::shared_ptr<Map> map = std::make_shared<Map>(it->second);
    return map;
}
void Maps::verify(const std::string &name) {
    this->load(name);
}