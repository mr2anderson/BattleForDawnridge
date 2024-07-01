/*
 *  Conquesta
 *  Copyright (C) 2023 mr2anderson
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
#include <cstdint>
#include <string>
#include <SFML/Graphics.hpp>
#include "Root.hpp"


#pragma once


class FontStorage {
public:
    static FontStorage* get() {
        if (FontStorage::singletone == nullptr) {
            FontStorage::singletone = new FontStorage();
        }
        return FontStorage::singletone;
    }

    void add(const std::string& name, const std::string& path);
    sf::Font* get(const std::string& name);
private:
    FontStorage() = default;
    FontStorage(const FontStorage& copy);
    static FontStorage* singletone;

    std::unordered_map<std::string, sf::Font> fonts;
};