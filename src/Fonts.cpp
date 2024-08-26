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
#include "Fonts.hpp"
#include "Root.hpp"
#include "CouldntOpenFont.hpp"


void Fonts::add(const std::string& name, const std::string& path) {
    if (!this->fonts[name].loadFromFile(Root::get().getDataRoot() + "/" + path)) {
        throw CouldntOpenFont(path);
    }
}
sf::Font* Fonts::get(const std::string& name) {
    auto it = this->fonts.find(name);
    if (it == this->fonts.end()) {
        std::cerr << "Invalid font uid: " << name << std::endl;
    }
    return &it->second;
}