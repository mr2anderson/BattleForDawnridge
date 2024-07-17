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


#include "PlayerPointerCircle.hpp"
#include "Textures.hpp"


PlayerPointerCircle::PlayerPointerCircle(float xInPixels, float yInPixels, uint32_t sx, uint32_t sy, uint8_t orientation) {
    this->xInPixels = xInPixels;
    this->yInPixels = yInPixels;
    this->sx = sx;
    this->orientation = orientation;
}
void PlayerPointerCircle::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(this->sprite, states);
}
void PlayerPointerCircle::setTypeBlue() {
    this->sprite.setTexture(*Textures::get()->get("blue"));
    this->recalcPosition();
}
void PlayerPointerCircle::setTypeGreen() {
    this->sprite.setTexture(*Textures::get()->get("green"));
    this->recalcPosition();
}
void PlayerPointerCircle::setTypePurple() {
    this->sprite.setTexture(*Textures::get()->get("purple"));
    this->recalcPosition();
}
void PlayerPointerCircle::recalcPosition() {
    this->sprite.setPosition(
        this->xInPixels + (this->orientation == ORIENTATION::LEFT_UP) * 5 + (this->orientation == ORIENTATION::RIGHT_UP) * (64 * this->sx - this->sprite.getLocalBounds().width),
        this->yInPixels - this->sprite.getGlobalBounds().height);
}