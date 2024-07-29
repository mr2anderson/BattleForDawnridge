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


#include "PoisonFog.hpp"
#include "Locales.hpp"
#include "Warrior.hpp"
#include "Textures.hpp"
#include "TilesetHandler.hpp"
#include "Parameters.hpp"
#include "GlobalRandomGenerator.hpp"
#include "GlobalClock.hpp"
#include "FocusOnEvent.hpp"


const uint32_t PoisonFog::TOTAL_TYPES = 1;


PoisonFog::PoisonFog() = default;
PoisonFog::PoisonFog(uint32_t x, uint32_t y, uint32_t playerId) : Effect(x, y, playerId) {
	this->type = GlobalRandomGenerator::get()->gen() % TOTAL_TYPES + 1;
}
std::string PoisonFog::getTextureName() const {
	return "poison_fog" + std::to_string(this->type);
}
sf::IntRect PoisonFog::getTextureRect() const {
	uint32_t totalFrames = Textures::get()->get(this->getTextureName())->getSize().x * Textures::get()->get(this->getTextureName())->getSize().y / 64 / this->getSX() / 64 / this->getSY();
	uint32_t currentFrame = GlobalClock::get()->getMs() / (1000 / totalFrames) % totalFrames;
	return TilesetHandler::get()->getTextureRect(64 * this->getSX(), 64 * this->getSY(), Textures::get()->get(this->getTextureName())->getSize().x, currentFrame);
}
std::string PoisonFog::getSoundName() const {
	return "poison_fog";
}
std::wstring PoisonFog::getDescription() const {
	return *Locales::get()->get("poison_fog_description") + std::to_wstring(Parameters::get()->getInt("poison_fog_damage"));
}
uint32_t PoisonFog::getLifetime() const {
	return Parameters::get()->getInt("poison_fog_lifetime");
}
Events PoisonFog::getActiveNewMoveEvent(MapState* state, uint32_t currentPlayerId) const {
	Events events;

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
		Warrior* w = state->getCollectionsPtr()->getWarrior(i);
		if (w->exist() and w->getPlayerId() == currentPlayerId and
                !w->isVehicle() and w->getX() == this->getX() and w->getY() == this->getY()) {
            events.add(std::make_shared<FocusOnEvent>(this->getX(), this->getY(), this->getSX(), this->getSY()));
			events = events + w->hit(Parameters::get()->getInt("poison_fog_damage"));
		}
	}

	return events;
}