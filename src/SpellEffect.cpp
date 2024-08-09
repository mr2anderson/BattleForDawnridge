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
#include "SpellEffect.hpp"
#include "Textures.hpp"


static constexpr float DST = 384;
static constexpr float TIME = 1;


SpellEffect::SpellEffect(const std::string& textureName, uint32_t x, uint32_t y) {
	this->sprite.setTexture(*Textures::get()->get(textureName));
	this->startX = 64 * x + 64 / 2 - this->sprite.getLocalBounds().width / 2;
	this->startY = 64 * (float)y - DST;
}
void SpellEffect::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->sprite, states);
}
void SpellEffect::update() {
	float dt = this->clock.getSecondsAsFloat();

	if (dt >= TIME) {
		this->finish();
	}
	else {
		float s = DST * std::pow(dt, 2) / std::pow(TIME, 2);
		this->sprite.setPosition(this->startX, this->startY + s);
	}
}