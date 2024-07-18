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


#include "VictoryCOnditionSpec.hpp"
#include "VictoryConditionBDestroyedEvent.hpp"
#include "Texts.hpp"


VictoryConditionSpec::VictoryConditionSpec() = default;
IBuildingSpec* VictoryConditionSpec::clone() const {
	return new VictoryConditionSpec(*this);
}
Events VictoryConditionSpec::getEventOnDestroy(MapState* state, uint32_t playerId) const {
	Events event;

	event.add(std::make_shared<VictoryConditionBDestroyedEvent>(playerId));

	return event;
}
std::vector<HorizontalSelectionWindowComponent> VictoryConditionSpec::getComponents(MapState* state, uint32_t playerId, const std::string &soundName, bool works, bool connectedToOrigin) {
	HorizontalSelectionWindowComponent component = {
		"star_icon",
		*Texts::get()->get("victory_condition_building_description"),
		false,
		Events()
	};
	return { component };
}
bool VictoryConditionSpec::isVictoryCondition() const {
	return true;
}