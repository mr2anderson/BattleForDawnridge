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


#include <cmath>
#include "PlayerPointerCircle.hpp"
#include "Textures.hpp"
#include "GlobalClock.hpp"


PlayerPointerCircle::PlayerPointerCircle(float xInPixels, float yInPixels, uint32_t sx, uint32_t sy) {
    this->xInPixels = xInPixels;
    this->yInPixels = yInPixels;
    this->sx = sx;
    this->sy = sy;
}
void PlayerPointerCircle::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Sprite sprite;
    sprite.setTexture(*Textures::get()->get(this->type));
    sprite.setPosition(this->getCurrentPosition());
    sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
    sprite.setScale(this->getCurrentScale(), this->getCurrentScale());

    target.draw(sprite, states);
}
void PlayerPointerCircle::setTypeBlue() {
    this->type = "blue";
}
void PlayerPointerCircle::setTypeGreen() {
    this->type = "green";
}
void PlayerPointerCircle::setTypePurple() {
    this->type = "purple";
}
float PlayerPointerCircle::getCurrentScale() const {
    return (0.5f + std::sin((long double)GlobalClock::get()->getMs() / (long double)(750 / 2)) / 4) * ((float)this->sx / 2);
}
sf::Vector2f PlayerPointerCircle::getCurrentPosition() const {
    return sf::Vector2f(
        this->xInPixels + (64 * this->sx) / 2,
        this->yInPixels + (64 * this->sy) / 2);
}