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


#include <boost/serialization/base_object.hpp>
#include "TerritoryConductorSpec.hpp"
#include "Locales.hpp"
#include "Building.hpp"
#include "HighlightColors.hpp"
#include "ArchiveType.hpp"


TerritoryConductorSpec::TerritoryConductorSpec() = default;
std::vector<BuildingHorizontalSelectionWindowComponent> TerritoryConductorSpec::getComponents(const Building* building, MapState* state) {
	if (!building->works() and !this->conductsIfNotWork()) {
		BuildingHorizontalSelectionWindowComponent component = {
			HorizontalSelectionWindowComponent("hammer_icon",
			*Locales::get()->get("does_not_expand_territory_if_hp_isnt_full"),
			false,
			Events()),
            true
		};
		return { component };
	}
	if (!building->connectedToOrigin(state)) {
		BuildingHorizontalSelectionWindowComponent component = {
             HorizontalSelectionWindowComponent(
			building->getTextureName(),
			*Locales::get()->get("does_not_lead_to_city_center"),
			false,
			Events()),
            true
		};
		return { component };
	}
	return {};
}
Events TerritoryConductorSpec::getHighlightEvent(const Building* building, MapState* state, uint8_t type) const {
	if (!building->connectedToOrigin(state) or (!building->works() and !this->conductsIfNotWork())) {
		return Events();
	}
	return this->AreaControllerSpec::getHighlightEvent(building, state, type);
}
bool TerritoryConductorSpec::isActiveConductor(const Building *building) const {
	return this->conductsIfNotWork() or building->works();
}
sf::Color TerritoryConductorSpec::getHighlightColor(uint32_t playerId) const {
    return HighlightColors::get()->getTerritoryExpandingColor(playerId);
}
uint8_t TerritoryConductorSpec::getHighlightType() const {
    return AreaControllerSpec::HIGHLIGHT_TYPE::TERRITORY;
}
NewMoveMainPriority TerritoryConductorSpec::getNewMoveMainPriority() const {
    return GO::NEW_MOVE_MAIN_PRIORITY_TERRITORY_CONDUCTOR;
}


BOOST_CLASS_EXPORT_IMPLEMENT(TerritoryConductorSpec)