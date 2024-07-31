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


#include <map>
#include "PoisonSpell.hpp"
#include "Locales.hpp"
#include "Warrior.hpp"
#include "PoisonFog.hpp"
#include "CreateEffectEvent.hpp"
#include "SubHpEvent.hpp"
#include "Parameters.hpp"


PoisonSpell::PoisonSpell() = default;
PoisonSpell::PoisonSpell(uint32_t playerId) : Spell(playerId) {

}
Spell* PoisonSpell::clone() const {
	return new PoisonSpell(*this);
}
std::wstring PoisonSpell::getDescription() const {
	return *Locales::get()->get("poison_spell_description") + std::to_wstring(Parameters::get()->getInt("poison_fog_damage"));
}
std::string PoisonSpell::getTextureName() const {
	return "poison_spell";
}
Resources PoisonSpell::getCost() const {
	return Parameters::get()->getResources("poison_spell_cost");
}
uint32_t PoisonSpell::getCreationTime() const {
	return Parameters::get()->getInt("poison_spell_creation_time");
}
sf::Color PoisonSpell::getPreviewColor() const {
	return sf::Color(50, 30, 0, 50);
}
uint32_t PoisonSpell::getRadius() const {
	return Parameters::get()->getInt("poison_spell_radius");
}
std::string PoisonSpell::getSoundName() const {
	return "poison_fog";
}
Events PoisonSpell::changeMap(MapState* state, uint32_t centerX, uint32_t centerY) {
	uint32_t xMin;
	if (centerX >= this->getRadius()) {
		xMin = centerX - this->getRadius();
	}
	else {
		xMin = 0;
	}

	uint32_t yMin;
	if (centerY >= this->getRadius()) {
		yMin = centerY - this->getRadius();
	}
	else {
		yMin = 0;
	}

	uint32_t xMax = centerX + this->getRadius();
	uint32_t yMax = centerY + this->getRadius();

	Events events;

	std::map<std::tuple<uint32_t, uint32_t>, Effect*> effects;

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalEffects(); i = i + 1) {
		Effect* effect = state->getCollectionsPtr()->getEffect(i);
		if (effect->exist()) {
			effects[std::make_tuple(effect->getX(), effect->getY())] = effect;
		}
	}

	for (uint32_t x = xMin; x <= xMax; x = x + 1) {
		for (uint32_t y = yMin; y <= yMax; y = y + 1) {
			auto it = effects.find(std::make_tuple(x, y));
			if (it != effects.end()) {
				events.add(std::make_shared<SubHpEvent>(it->second, it->second->getHP()));
			}
			events.add(std::make_shared<CreateEffectEvent>(new PoisonFog(x, y, this->getPlayerId())));
		}
	}

	return events;
}


BOOST_CLASS_EXPORT_IMPLEMENT(PoisonSpell)