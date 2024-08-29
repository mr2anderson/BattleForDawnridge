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
#include <SFML/Audio.hpp>
#include <optional>


#pragma once


class Music {
public:
    static Music& get() {
        static Music instance;
        return instance;
    }

    void add(const std::string& name, const std::string& path);
    void play(const std::string& name);
    sf::Music::Status getStatus(const std::string &name);
    void loadVolume();
    uint32_t getVolume() const;
    void setVolume(uint32_t newVolume);
    void stop();
private:
    Music();
    Music(const Music& copy);

    uint32_t volume;
    std::optional<std::string> lastSoundtrack;
    std::unordered_map<std::string, sf::Music> music;
};