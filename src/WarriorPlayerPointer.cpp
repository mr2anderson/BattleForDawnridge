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


WarriorPlayerPointer::WarriorPlayerPointer(float xInPixels, float yInPixels, uint8_t state) {
    this->sprite.setPosition(xInPixels, yInPixels);
    this->state = state;
}
void WarriorPlayerPointer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(this->sprite, states);
}
void WarriorPlayerPointer::setTypeBlue() {
    this->sprite.setTexture(*Textures::get().get("warrior_blue" + this->getTextureNameSuffix()));
}
void WarriorPlayerPointer::setTypeGreen() {
    this->sprite.setTexture(*Textures::get().get("warrior_green" + this->getTextureNameSuffix()));
}
void WarriorPlayerPointer::setTypePurple() {
    this->sprite.setTexture(*Textures::get().get("warrior_purple" + this->getTextureNameSuffix()));
}
std::string WarriorPlayerPointer::getTextureNameSuffix() const {
    switch (this->state) {
    case ENEMY_MOVE:
        return "";
    case DOES_NOT_HAVE_MOVES:
        return "0";
    case HAS_MOVES:
        return "1";
    }
    return "";
}