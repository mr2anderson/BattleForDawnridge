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


#include "HealSpell.hpp"
#include "AddHpEvent.hpp"
#include "CreateEEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "Locales.hpp"
#include "Warrior.hpp"
#include "Parameters.hpp"
#include "HPFlyingE.hpp"


HealSpell::HealSpell() = default;
HealSpell::HealSpell(uint32_t playerId) : Spell(playerId) {

}
std::shared_ptr<Spell> HealSpell::clone() const {
    return std::make_shared<HealSpell>(*this);
}
StringLcl HealSpell::getDescription() const {
    return StringLcl("{heal_spell_description}");
}
std::string HealSpell::getTextureName() const {
    return "heal_spell";
}
Resources HealSpell::getCost() const {
    return Parameters::get().getResources("heal_spell_cost");
}
uint32_t HealSpell::getCreationTime() const {
    return Parameters::get().getInt("heal_spell_creation_time");
}
sf::Color HealSpell::getPreviewColor() const {
    return sf::Color(100, 100, 0, 30);
}
uint32_t HealSpell::getRadius() const {
    return Parameters::get().getInt("heal_spell_radius");
}
std::string HealSpell::getSoundName() const {
    return "heal_spell";
}
Events HealSpell::changeMap(MapState* state, uint32_t centerX, uint32_t centerY) {
    Events events;

    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
        std::shared_ptr<Warrior> w = state->getCollectionsPtr()->getWarrior(i);
        if (w->exist() and w->getPlayerId() == this->getPlayerId()) {
            if (std::max(centerX, w->getX()) - std::min(centerX, w->getX()) > this->getRadius()) {
                continue;
            }
            if (std::max(centerY, w->getY()) - std::min(centerY, w->getY()) > this->getRadius()) {
                continue;
            }
            events = events + w->heal();
        }
    }

    return events;
}


BOOST_CLASS_EXPORT_IMPLEMENT(HealSpell)