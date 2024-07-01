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
#include <SFML/Audio.hpp>
#include "Root.hpp"


#pragma once


class MusicStorage {
public:
    static MusicStorage *get() {
        if (MusicStorage::singletone == nullptr) {
            MusicStorage::singletone = new MusicStorage();
        }
        return MusicStorage::singletone;
    }

    void add(const std::string& name, const std::string& path);
    [[nodiscard]] sf::Music *get(const std::string& name);
private:
    MusicStorage() = default;
    MusicStorage(const MusicStorage& copy);
    static MusicStorage *singletone;

    std::unordered_map<std::string, sf::Music> music;
};