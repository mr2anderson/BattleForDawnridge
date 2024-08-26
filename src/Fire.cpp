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


#include "Fire.hpp"
#include "Textures.hpp"
#include "GlobalClock.hpp"


const uint32_t Fire::TOTAL_FRAMES = 25;


Fire::Fire() = default;
Fire::Fire(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->scaleX = 64.f * (float)sx / 128.f;
	this->scaleY = 64.f * (float)sy / 128.f;
	this->x = 64 * x;
	this->y = 64 * y;
}
void Fire::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprite;
	sprite.setScale(this->scaleX, this->scaleY);
	sprite.setTexture(*Textures::get().get(this->getTextureName()));
	sprite.setPosition(this->x, this->y);
	target.draw(sprite, states);
}
uint32_t Fire::getCurrentFrame() const {
	return GlobalClock::get().getMs() / (1000 / TOTAL_FRAMES) % TOTAL_FRAMES + 1;
}
std::string Fire::getTextureName() const {
	return "fire" + std::to_string(this->getCurrentFrame());
}