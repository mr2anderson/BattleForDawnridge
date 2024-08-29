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


#include <iostream>
#include <filesystem>
#include <fstream>
#include "Music.hpp"
#include "Root.hpp"
#include "CouldntOpenMusic.hpp"


Music::Music() {
    this->volume = 15;
}
void Music::add(const std::string& name, const std::string& path) {
    if (!this->music[name].openFromFile(Root::get().getDataRoot() + "/" + path)) {
        throw CouldntOpenMusic(path);
    }
}
void Music::play(const std::string &name) {
    auto it = this->music.find(name);
    if (it == this->music.end()) {
        std::cerr << "Invalid music uid: " << name << std::endl;
    }
    else {
        this->stop();
        it->second.setVolume(this->volume);
        it->second.play();
        this->lastSoundtrack = name;
    }
}
sf::Music::Status Music::getStatus(const std::string &name) {
    auto it = this->music.find(name);
    if (it == this->music.end()) {
        std::cerr << "Invalid music uid: " << name << std::endl;
    }
    return it->second.getStatus();
}
void Music::loadVolume() {
    std::ifstream file(Root::get().getUserdataRoot() + "/music_volume.cfg");
    if (file.is_open()) {
        std::string buff;
        std::getline(file, buff);
        this->volume = std::stoi(buff);
        file.close();
    }
}
uint32_t Music::getVolume() const {
    return this->volume;
}
void Music::setVolume(uint32_t newVolume) {
    this->volume = newVolume;
    if (this->lastSoundtrack.has_value()) {
        this->music.at(this->lastSoundtrack.value()).setVolume(this->volume);
    }
    if (!std::filesystem::is_directory(Root::get().getUserdataRoot())) {
        std::filesystem::create_directories(Root::get().getUserdataRoot());
    }
    std::ofstream file(Root::get().getUserdataRoot() + "/music_volume.cfg");
    file << std::to_string(this->volume);
    file.close();
}
void Music::stop() {
    if (this->lastSoundtrack.has_value()) {
        this->music[this->lastSoundtrack.value()].stop();
    }
}