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


#include <random>
#include "PoisonFog.hpp"
#include "Texts.hpp"
#include "Warrior.hpp"
#include "Textures.hpp"
#include "TilesetHandler.hpp"
#include "Balance.hpp"


const uint32_t PoisonFog::TOTAL_TYPES = 1;


PoisonFog::PoisonFog() = default;
PoisonFog::PoisonFog(uint32_t x, uint32_t y, uint32_t playerId) : Effect(x, y, playerId) {
	std::random_device rd;
	this->type = rd() % TOTAL_TYPES + 1;
}
std::string PoisonFog::getTextureName() const {
	return "poison_fog" + std::to_string(this->type);
}
sf::IntRect PoisonFog::getTextureRect() const {
	uint32_t totalFrames = Textures::get()->get(this->getTextureName())->getSize().x * Textures::get()->get(this->getTextureName())->getSize().y / 64 / this->getSX() / 64 / this->getSY();
	uint32_t currentFrame = this->animationClock.getElapsedTime().asMilliseconds() / (1000 / totalFrames) % totalFrames;
	return TilesetHandler::get()->getTextureRect(64 * this->getSX(), 64 * this->getSY(), Textures::get()->get(this->getTextureName())->getSize().x, currentFrame);
}
std::string PoisonFog::getSoundName() const {
	return "poison_fog";
}
std::wstring PoisonFog::getDescription() const {
	return *Texts::get()->get("poison_fog_description") + std::to_wstring(Balance::get()->getInt("poison_fog_damage"));
}
uint32_t PoisonFog::getLifetime() const {
	return Balance::get()->getInt("poison_fog_lifetime");
}
Events PoisonFog::getActiveNewMoveEvent(MapState* state, uint32_t currentPlayerId) const {
	Events events;

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
		Warrior* w = state->getCollectionsPtr()->getWarrior(i);
		if (w->exist() and w->getPlayerId() == currentPlayerId and w->getX() == this->getX() and w->getY() == this->getY()) {
			events = events + w->hit(Balance::get()->getInt("poison_fog_damage"), std::nullopt);
		}
	}

	return events;
}