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
#include "Textures.hpp"
#include "Root.hpp"
#include "CouldntOpenTexture.hpp"


void Textures::add(const std::string& name, const std::string& path) {
    std::string fpath = std::filesystem::absolute(Root::get().getDataRoot() + "/" + path).generic_string();
    std::cout << fpath << std::endl;
    if (!this->textures[name].loadFromFile(fpath)) {
        throw CouldntOpenTexture(path);
    }
}
void Textures::add(const std::string& name, const sf::Texture& texture) {
    this->textures[name] = texture;
}
sf::Texture *Textures::get(const std::string& name) {
    auto it = this->textures.find(name);
    if (it == this->textures.end()) {
        std::cerr << "Invalid texture uid: " << name << std::endl;
    }
    return &it->second;
}