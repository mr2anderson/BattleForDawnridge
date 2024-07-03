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


#include <ctime>
#include "Music.hpp"


#pragma once


class Playlist {
public:
    static Playlist *get() {
        if (Playlist::singletone == nullptr) {
            Playlist::singletone = new Playlist(SOUNDTRACKS_N);
        }
        return Playlist::singletone;
    }

    void update();
    void restartMusic();

    static constexpr uint32_t SOUNDTRACKS_N = 10;
private:
    Playlist() = default;
    Playlist(uint32_t number) {
        this->number = number;
        this->index = (int32_t)(time(nullptr) % this->number);
    }
    Playlist(const Playlist& copy);
    static Playlist *singletone;

    uint32_t number;
    uint32_t index;

    static constexpr uint32_t MUSIC_VOLUME = 40;
};