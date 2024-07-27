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
#include "Projectile.hpp"
#include "Textures.hpp"


Projectile::Projectile() = default;
void Projectile::setSrc(float xInPixels, float yInPixels) {
    this->x1 = xInPixels - Textures::get()->get(this->getTextureName())->getSize().x / 2;
    this->y1 = yInPixels - Textures::get()->get(this->getTextureName())->getSize().y / 2;
}
void Projectile::setDst(float xInPixels, float yInPixels) {
    xInPixels = xInPixels - Textures::get()->get(this->getTextureName())->getSize().x / 2;
    yInPixels = yInPixels - Textures::get()->get(this->getTextureName())->getSize().y / 2;

    float dx = xInPixels - this->x1;
    float dy = yInPixels - this->y1;

    this->vx = dx / this->getTime();
    this->vy = dy / this->getTime();

    double sin = std::abs(dy) / std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    float asin = -std::asin(sin) * 180 / M_PI + 90;

    if (dy > 0) {
        if (dx > 0) {
            this->rotation = 180 + asin;
        }
        else {
            this->rotation = 180 - asin;
        }
    }
    else {
        if (dx > 0) {
            this->rotation = 360 - asin;
        }
        else {
            this->rotation = asin;
        }
    }
    this->rotation = (360 - this->rotation); // SFML uses clocking order
}
void Projectile::run(uint32_t windowW, uint32_t windowH) {
    this->clock.restart();
}
void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    float dt = this->clock.getElapsedTime().asSeconds();

    sf::Sprite sprite;
    sprite.setTexture(*Textures::get()->get(this->getTextureName()));
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setPosition(this->x1 + dt * this->vx, this->y1 + dt * this->vy);
    sprite.setRotation(this->rotation);
    target.draw(sprite, states);
}
Events Projectile::click() {
    return Events();
}
void Projectile::update() {
    float dt = this->clock.getElapsedTime().asSeconds();
    if (dt >= this->getTime()) {
        this->finish();
    }
}