/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Root.hpp"


#pragma once


class Textures {
public:
    static Textures *get() {
        if (Textures::singletone == nullptr) {
            Textures::singletone = new Textures();
        }
        return Textures::singletone;
    }

    void add(const std::string& name, const std::string& path);
    sf::Texture *get(const std::string& name);
private:
    Textures() = default;
    Textures(const Textures& copy);
    static Textures *singletone;

    std::unordered_map<std::string, sf::Texture> textures;
};