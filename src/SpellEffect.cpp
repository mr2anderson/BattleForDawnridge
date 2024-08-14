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


SpellEffect::SpellEffect() = default;
SpellEffect::SpellEffect(const std::string& textureName, uint32_t x, uint32_t y) {
	this->textureName = textureName;
	this->startX = 64 * x + 64 / 2;
	this->startY = 64 * (float)y - DST;
}
void SpellEffect::onRestart() {
	this->clock.restart();
}
void SpellEffect::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprite;
	sprite.setTexture(*Textures::get()->get(this->textureName));
	sprite.setPosition(this->startX, this->startY + DST * std::pow(this->clock.getSecondsAsFloat(), 2) / std::pow(TIME, 2));
	sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
	target.draw(sprite, states);
}
void SpellEffect::update() {
	if (this->clock.getSecondsAsFloat() >= TIME) {
		this->finish();
	}
}


BOOST_CLASS_EXPORT_IMPLEMENT(SpellEffect)