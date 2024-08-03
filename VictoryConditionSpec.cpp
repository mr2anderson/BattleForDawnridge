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


#include "VictoryConditionSpec.hpp"
#include "MarkPlayerAsInactiveEvent.hpp"
#include "Locales.hpp"
#include "Building.hpp"
#include "FocusOnEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "ImageFlyingE.hpp"
#include "CreateEEvent.hpp"
#include "IncreaseVCSMoveCtrEvent.hpp"


VictoryConditionSpec::VictoryConditionSpec() {
    this->moveCtr = 0;
}
IBuildingSpec* VictoryConditionSpec::clone() const {
	return new VictoryConditionSpec(*this);
}
Events VictoryConditionSpec::getActiveNewMoveEvent(const Building* b, MapState* state) {
    Events events;

    events.add(std::make_shared<FocusOnEvent>(b->getX(), b->getY(), b->getSX(), b->getSY()));
    events.add(std::make_shared<PlaySoundEvent>("bell"));
    events.add(std::make_shared<IncreaseVCSMoveCtrEvent>(this));

    std::shared_ptr<ImageFlyingE> flyingE = std::make_shared<ImageFlyingE>("bell", b->getX(), b->getY(), b->getSX(), b->getSY());
    events.add(std::make_shared<CreateEEvent>(flyingE));

    return events;
}
Events VictoryConditionSpec::getEventOnDestroy(const Building *b, MapState* state) const {
	Events event;

    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
        Building* building = state->getCollectionsPtr()->getBuilding(i);
        if (building != b and building->getPlayerId() == b->getPlayerId() and building->exist() and building->isVictoryCondition()) {
            return event;
        }
    }

	event.add(std::make_shared<MarkPlayerAsInactiveEvent>(b->getPlayerId()));

	return event;
}
std::vector<BuildingHorizontalSelectionWindowComponent> VictoryConditionSpec::getComponents(const Building *b, MapState* state) {
	BuildingHorizontalSelectionWindowComponent component = {
		HorizontalSelectionWindowComponent("bell",
		*Locales::get()->get("victory_condition_building_description") + std::to_wstring(this->moveCtr),
		false,
		Events()),
        true
	};
	return { component };
}
bool VictoryConditionSpec::isVictoryCondition() const {
	return true;
}
NewMoveMainPriority VictoryConditionSpec::getNewMoveMainPriority() const {
    return GO::NEW_MOVE_MAIN_PRIORITY_VICTORY_CONDITION;
}
void VictoryConditionSpec::increaseMoveCtr() {
    this->moveCtr = this->moveCtr + 1;
}


BOOST_CLASS_EXPORT_IMPLEMENT(VictoryConditionSpec)