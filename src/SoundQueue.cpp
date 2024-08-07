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


#include "SoundQueue.hpp"


SoundQueue *SoundQueue::singletone = nullptr;


SoundQueue::SoundQueue() = default;
void SoundQueue::push(sf::SoundBuffer *soundbuffer) {
    this->removeOldSounds();
    this->data.emplace_back();
    this->data.back().setBuffer(*soundbuffer);
    this->data.back().play();
}
void SoundQueue::clear() {
    for (auto& sound : this->data) {
        sound.stop();
    }
    this->data.clear();
}
void SoundQueue::removeOldSounds() {
    while (!this->data.empty() and this->data.front().getStatus() != sf::Sound::Status::Playing) {
        this->data.erase(this->data.begin());
    }
}