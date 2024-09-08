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


#include "IWarriorHealerSpec.hpp"
#include "Building.hpp"
#include "Warrior.hpp"
#include "FocusOnEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "HPFlyingE.hpp"
#include "HighlightColors.hpp"
#include "Parameters.hpp"



Events IWarriorHealerSpec::getActiveNewMoveEvent(std::shared_ptr<const Building> b, MapState *state) {
	if (!b->works()) {
		return Events();
	}

	Events events;

	bool first = true;
	HashTableMapPosition<uint32_t> available = this->getAvailable(b->getX(), b->getY(), b->getSX(), b->getSY(), b->getPlayerId(), state);
	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
		std::shared_ptr<Warrior>  w = state->getCollectionsPtr()->getWarrior(i);
		if (w->exist() and w->getPlayerId() == b->getPlayerId() and !w->wasHealed() and
                w->isVehicle() == this->healVehicles() and w->getHP() != w->getMaxHP() and IAreaControllerSpec::IN_RADIUS(available, w, IAreaControllerSpec::IN_RADIUS_TYPE::FULLY)) {
			if (first) {
				events.add(std::make_shared<FocusOnEvent>(b->getX(), b->getY(), b->getSX(), b->getSY()));
				first = false;
			}

            events = events + w->heal();
		}
	}

	return events;
}
std::vector<BuildingHorizontalSelectionWindowComponent> IWarriorHealerSpec::getComponents(std::shared_ptr<const Building>  b, MapState* state) {
	BuildingHorizontalSelectionWindowComponent component;

	if (b->works()) {
		component = {
			HorizontalSelectionWindowComponent(this->getHealTextureName(),
            StringLcl("{this_building_heals_warriors}") + std::to_string(Parameters::get().getInt("healing_speed")),
			false,
			Events()),
            true
		};
	}
	else {
		component = {
			HorizontalSelectionWindowComponent(  "hammer_icon",
			StringLcl("{does_not_heal_if_hp_isnt_full}"),
			false,
			Events()),
            true
		};
	}

	return { component };
}
uint32_t IWarriorHealerSpec::getRadius() const {
	return 1;
}
sf::Color IWarriorHealerSpec::getHighlightColor(uint32_t playerId) const {
    return HighlightColors::get().getBuildingHealColor(playerId);
}
uint8_t IWarriorHealerSpec::getHighlightType() const {
    return IAreaControllerSpec::HIGHLIGHT_TYPE::OTHER;
}
bool IWarriorHealerSpec::healVehicles() const {
    return false;
}
NewMoveMainPriority IWarriorHealerSpec::getNewMoveMainPriority() const {
    return GO::NEW_MOVE_MAIN_PRIORITY_HEALER;
}


BOOST_CLASS_EXPORT_IMPLEMENT(IWarriorHealerSpec)