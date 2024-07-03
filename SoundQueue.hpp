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


#include <list>
#include <SFML/Audio.hpp>


#pragma once


class SoundQueue {
public:
    static SoundQueue *get() {
        if (SoundQueue::singletone == nullptr) {
            SoundQueue::singletone = new SoundQueue();
        }
        return SoundQueue::singletone;
    }

    void push(sf::SoundBuffer *soundbuffer);
    void clear();
private:
    SoundQueue() = default;
    SoundQueue(const SoundQueue& copy);
    static SoundQueue *singletone;

    std::list<sf::Sound> data;

    void removeOldSounds();
};