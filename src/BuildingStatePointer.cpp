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
#include "BuildingStatePointer.hpp"
#include "Textures.hpp"
#include "GlobalClock.hpp"


const uint32_t BuildingStatePointer::TOTAL_HP_POINTERS = 21;


BuildingStatePointer::BuildingStatePointer(float xInPixels, float yInPixels, uint32_t sx, uint32_t sy, uint32_t currentHP, uint32_t maxHP) {
    this->xInPixels = xInPixels;
    this->yInPixels = yInPixels;

    this->sx = sx;
    this->sy = sy;

    this->current = currentHP;
    this->max = maxHP;
}
void BuildingStatePointer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Sprite sprite;
    sprite.setTexture(*Textures::get()->get(this->type));
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setScale(this->getCurrentScale(), this->getCurrentScale());
    sprite.setPosition(this->xInPixels + 8 * this->sx, this->yInPixels + 8 * this->sx);
    target.draw(sprite, states);

    uint32_t id = (TOTAL_HP_POINTERS - 1) * this->current / this->max;
    sprite = sf::Sprite();
    sprite.setTexture(*Textures::get()->get("building_hp_pointer" + std::to_string(id)));
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setScale(this->getCurrentScale(), this->getCurrentScale());
    sprite.setPosition(this->xInPixels + 8 * this->sx, this->yInPixels + 16 * this->sx);
    target.draw(sprite, states);
}
void BuildingStatePointer::setTypeBlue() {
    this->type = "blue";
}
void BuildingStatePointer::setTypeGreen() {
    this->type = "green";
}
void BuildingStatePointer::setTypePurple() {
    this->type = "purple";
}
float BuildingStatePointer::getCurrentScale() const {
    return (0.5f + std::sin((long double)GlobalClock::get()->getMs() / (long double)(750 / 2)) / 4) * std::pow((float)this->sx / 2, 2);
}