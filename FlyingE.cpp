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


FlyingE::FlyingE(const std::string& picture, const std::string& sound, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->picture = picture;
	this->sound = sound;
	this->x = x;
	this->y = y;
	this->sx = sx;
	this->sy = sy;
}
Events FlyingE::run(uint32_t windowW, uint32_t windowH) {
	this->clock.restart();
	this->dst = windowH / 4;

	Events uiEvent;
	uiEvent.add(std::make_shared<PlaySoundEvent>(this->sound));

	return uiEvent;
}
void FlyingE::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	float t = this->clock.getElapsedTime().asSeconds();

	sf::Sprite sprite;
	sprite.setScale(0.5, 0.5);
	sprite.setTexture(*Textures::get()->get(this->picture));
	sprite.setColor(this->getTransparencyLevel(t));
	sprite.setPosition(this->getPosition(t));
	target.draw(sprite, states);
}
Events FlyingE::click() {
	return Events();
}
void FlyingE::update() {
	if (this->clock.getElapsedTime().asSeconds() >= TIME) {
		this->finish();
	}
}
sf::Color FlyingE::getTransparencyLevel(float t) const {
	sf::Color color = sf::Color::White;
	color.a = 255 - std::min(255.f, 255 * t / TIME);
	return color;
}
sf::Vector2f FlyingE::getPosition(float t) const {
	float a = 2 * (this->dst - V0 * TIME) / std::pow(TIME, 2);

	float startX = 32 * this->x + 32 * (float)this->sx / 2 - 64 / 2 / 2;
	float startY = 32 * this->y + 32 * (float)this->sy / 2 - 64 / 2 / 2;
	
	float currentX = startX;
	float currentY = startY - V0 * t - a * std::pow(t, 2) / 2;

	return sf::Vector2f(currentX, currentY);
}