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


#include "WarriorHealerSpec.hpp"
#include "Building.hpp"
#include "Warrior.hpp"
#include "FocusOnEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "AddHpEvent.hpp"
#include "CreateEEvent.hpp"
#include "HPFlyingE.hpp"
#include "Locales.hpp"
#include "HighlightColors.hpp"


WarriorHealerSpec::WarriorHealerSpec() = default;
Events WarriorHealerSpec::getActiveNewMoveEvent(const Building *b, MapState *state) {
	if (!b->works()) {
		return Events();
	}

	Events events;

	bool first = true;
	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
		Warrior* w = state->getCollectionsPtr()->getWarrior(i);
		if (w->exist() and w->getPlayerId() == b->getPlayerId() and
                w->isVehicle() == this->healVehicles() and w->getHP() != w->getMaxHP() and this->inRadius(b, state, w->getX(), w->getY(), w->getSX(), w->getSY())) {
			if (first) {
				events.add(std::make_shared<FocusOnEvent>(b->getX(), b->getY(), b->getSX(), b->getSY()));
				first = false;
			}

			events.add(std::make_shared<PlaySoundEvent>("heal"));

			std::shared_ptr<HPFlyingE> flyingE = std::make_shared<HPFlyingE>(std::min(this->getHealingSpeed(), w->getMaxHP() - w->getHP()), true, w->getX(), w->getY(), w->getSX(), w->getSY());
			events.add(std::make_shared<CreateEEvent>(flyingE));

			events.add(std::make_shared<AddHpEvent>(w, this->getHealingSpeed()));
		}
	}

	return events;
}
std::vector<BuildingHorizontalSelectionWindowComponent> WarriorHealerSpec::getComponents(const Building* b, MapState* state) {
	BuildingHorizontalSelectionWindowComponent component;

	if (b->works()) {
		component = {
			HorizontalSelectionWindowComponent(this->getHealTextureName(),
            *Locales::get()->get("this_building_heals_warriors") + std::to_wstring(this->getHealingSpeed()),
			false,
			Events()),
            true
		};
	}
	else {
		component = {
			HorizontalSelectionWindowComponent("hammer_icon",
			*Locales::get()->get("does_not_heal_if_hp_isnt_full"),
			false,
			Events()),
            true
		};
	}

	return { component };
}
uint32_t WarriorHealerSpec::getRadius() const {
	return 0;
}
sf::Color WarriorHealerSpec::getHighlightColor(uint32_t playerId) const {
    return HighlightColors::get()->getBuildingHealColor(playerId);
}
uint8_t WarriorHealerSpec::getHighlightType() const {
    return AreaControllerSpec::HIGHLIGHT_TYPE::OTHER;
}
bool WarriorHealerSpec::healVehicles() const {
    return false;
}