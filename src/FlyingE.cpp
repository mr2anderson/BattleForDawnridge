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


#include "FlyingE.hpp"


static constexpr float DST = 128;
static constexpr float TIME = 0.625;


FlyingE::FlyingE() = default;
FlyingE::FlyingE(float x, float y) {
    this->startX = x;
    this->startY = y;
    this->clock.restart();
}
void FlyingE::onRestart() {
    this->clock.restart();
}
void FlyingE::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(*this->getDrawable(this->getPosition(), this->getTransparencyLevel()), states);
}
void FlyingE::update() {
    if (this->clock.getSecondsAsFloat() >= TIME) {
        this->finish();
    }
}
sf::Color FlyingE::getTransparencyLevel() const {
    sf::Color color = sf::Color::White;
    color.a = 255 - std::min(255.f, 255 * this->clock.getSecondsAsFloat() / TIME);
    return color;
}
sf::Vector2f FlyingE::getPosition() const {
    float v = DST / TIME;

    float currentX = this->startX;
    float currentY = this->startY - v * this->clock.getSecondsAsFloat();

    return sf::Vector2f(currentX, currentY);
}


BOOST_CLASS_EXPORT_IMPLEMENT(FlyingE)