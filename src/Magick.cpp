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


#include "Magick.hpp"
#include "Textures.hpp"
#include "ClosePopUpElementEvent.hpp"


Magick::Magick() = default;
void Magick::onRestart() {
    this->clock.restart();
}
void Magick::setTarget(float xInPixels, float yInPixels) {
    this->x = xInPixels;
    this->y = yInPixels;
}
void Magick::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Sprite sprite;
    sprite.setTexture(*Textures::get().get(this->getTextureName()));
    sprite.setPosition(this->x, this->y);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height);
    target.draw(sprite, states);
}
Events Magick::update() {
    Events result;

    if (this->clock.getSecondsAsFloat() >= this->getTime()) {
        result.add(std::make_shared<ClosePopUpElementEvent>());
    }

    return result;
}
uint32_t Magick::getMS() const {
    return this->clock.getMS();
}


BOOST_CLASS_EXPORT_IMPLEMENT(Magick)