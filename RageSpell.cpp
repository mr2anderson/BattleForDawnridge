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


#include "RageSpell.hpp"
#include "EnableWarriorRageModeEvent.hpp"
#include "Texts.hpp"
#include "Warrior.hpp"


RageSpell::RageSpell(uint32_t playerId) : Spell(playerId) {

}
Spell* RageSpell::clone() const {
	return new RageSpell(*this);
}
std::wstring RageSpell::getDescription() const {
	return *Texts::get()->get("rage_spell_description");
}
std::string RageSpell::getTextureName() const {
	return "rage_spell";
}
Resources RageSpell::getCost() const {
	Resources cost;
	cost.plus(Resource("iron", 20000));
	return cost;
}
uint32_t RageSpell::getCreationTime() const {
	return 6;
}
sf::Color RageSpell::getPreviewColor() const {
	return sf::Color(75, 0, 130, 30);
}
uint32_t RageSpell::getRadius() const {
	return 2;
}
std::string RageSpell::getSoundName() const {
	return "rage_spell";
}
Events RageSpell::changeMap(MapState* state, uint32_t centerX, uint32_t centerY) {
	Events events;

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
		Warrior* w = state->getCollectionsPtr()->getWarrior(i);
		if (w->exist() and w->getPlayerId() == this->getPlayerId()) {
			if (std::max(centerX, w->getX()) - std::min(centerX, w->getX()) > this->getRadius()) {
				continue;
			}
			if (std::max(centerY, w->getY()) - std::min(centerY, w->getY()) > this->getRadius()) {
				continue;
			}
			events.add(std::make_shared<EnableWarriorRageModeEvent>(w));
		}
	}

	return events;
}