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


#include "WarriorPlayerPointer.hpp"
#include "Textures.hpp"


WarriorPlayerPointer::WarriorPlayerPointer(float xInPixels, float yInPixels, bool color) {
    this->sprite.setPosition(xInPixels, yInPixels);
    this->color = color;
}
void WarriorPlayerPointer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(this->sprite, states);
}
void WarriorPlayerPointer::setTypeBlue() {
    if (this->color) {
        this->sprite.setTexture(*Textures::get()->get("warrior_blue"));
    }
    else {
        this->sprite.setTexture(*Textures::get()->get("warrior_blue_no_moves"));
    }
}
void WarriorPlayerPointer::setTypeGreen() {
    if (this->color) {
        this->sprite.setTexture(*Textures::get()->get("warrior_green"));
    }
    else {
        this->sprite.setTexture(*Textures::get()->get("warrior_green_no_moves"));
    }
}
void WarriorPlayerPointer::setTypePurple() {
    if (this->color) {
        this->sprite.setTexture(*Textures::get()->get("warrior_purple"));
    }
    else {
        this->sprite.setTexture(*Textures::get()->get("warrior_purple_no_moves"));
    }
}