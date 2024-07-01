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


#include "Playlist.hpp"


Playlist *Playlist::singletone = nullptr;


void Playlist::update() {
    if (MusicStorage::get()->get(std::to_string(this->index))->getStatus() == sf::Music::Status::Playing) {
        return;
    }
    this->index = (this->index + 1) % this->number;
    MusicStorage::get()->get(std::to_string(this->index))->play();
    MusicStorage::get()->get(std::to_string(this->index))->setVolume(60);
}
void Playlist::restartMusic() {
    for (int32_t i = 0; i < this->number; i = i + 1) {
        MusicStorage::get()->get(std::to_string(this->index))->stop();
    }
}