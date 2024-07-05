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


#include "Playlist.hpp"


Playlist *Playlist::singletone = nullptr;


void Playlist::update() {
    if (Music::get()->get(std::to_string(this->index))->getStatus() == sf::Music::Status::Playing) {
        return;
    }
    this->index = (this->index + 1) % this->number;
    Music::get()->get(std::to_string(this->index))->play();
    Music::get()->get(std::to_string(this->index))->setVolume(MUSIC_VOLUME);
}
void Playlist::restartMusic() {
    for (uint32_t i = 0; i < this->number; i = i + 1) {
        Music::get()->get(std::to_string(this->index))->stop();
    }
}