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
#include "Music.hpp"
#include "GlobalRandomGenerator32.hpp"


const uint32_t Playlist::SOUNDTRACKS_N = 7;


Playlist::Playlist() {
    this->index = GlobalRandomGenerator32::get().gen() % SOUNDTRACKS_N;
}
void Playlist::update() {
    if (Music::get().getStatus(std::to_string(this->index)) == sf::Music::Status::Playing) {
        return;
    }
    this->index = (this->index + 1) % SOUNDTRACKS_N;
    Music::get().play(std::to_string(this->index));
}