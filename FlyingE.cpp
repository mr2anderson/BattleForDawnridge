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
#include "FlyingE.hpp"
#include "Textures.hpp"
#include "PlaySoundEvent.hpp"


const float FlyingE::TIME = 0.5f;
const float FlyingE::V0 = 64;


FlyingE::FlyingE() = default;
void FlyingE::run(uint32_t windowW, uint32_t windowH) {
	this->clock.restart();
	this->dst = windowH / 4;
}
void FlyingE::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}
Events FlyingE::click() {
	return Events();
}
void FlyingE::update() {
    float t = this->clock.getElapsedTime().asSeconds();

    this->sprite.setColor(this->getTransparencyLevel(t));
    this->sprite.setPosition(this->getPosition(t));

	if (this->clock.getElapsedTime().asSeconds() >= TIME) {
		this->finish();
	}
}
void FlyingE::setSprite(const sf::Sprite &sprite1) {
    this->sprite = sprite1;
    this->spriteStartX = this->sprite.getPosition().x;
    this->spriteStartY = this->sprite.getPosition().y;
}
sf::Color FlyingE::getTransparencyLevel(float t) const {
	sf::Color color = sf::Color::White;
	color.a = 255 - std::min(255.f, 255 * t / TIME);
	return color;
}
sf::Vector2f FlyingE::getPosition(float t) const {
	float a = 2 * (this->dst - V0 * TIME) / std::pow(TIME, 2);
	
	float currentX = this->spriteStartX;
	float currentY = this->spriteStartY - V0 * t - a * std::pow(t, 2) / 2;

	return sf::Vector2f(currentX, currentY);
}