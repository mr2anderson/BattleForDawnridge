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


#include <list>
#include <SFML/Audio.hpp>


#pragma once


class SoundQueue {
public:
    static SoundQueue& get() {
        static SoundQueue instance;
        return instance;
    }

    void push(sf::SoundBuffer *soundbuffer);
    void clear();
    void loadVolume();
    uint32_t getVolume() const;
    void setVolume(uint32_t newVolume);
private:
    SoundQueue();
    SoundQueue(const SoundQueue& copy);

    uint32_t volume;

    std::list<sf::Sound> data;

    void removeOldSounds();
};