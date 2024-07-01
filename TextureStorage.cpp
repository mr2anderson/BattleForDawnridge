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


#include "TextureStorage.hpp"


TextureStorage *TextureStorage::singletone = nullptr;


void TextureStorage::add(const std::string& name, const std::string& path) {
    this->textures[name].loadFromFile(std::string(ROOT) + "/" + path);
}
sf::Texture *TextureStorage::get(const std::string& name) {
    return &this->textures[name];
}