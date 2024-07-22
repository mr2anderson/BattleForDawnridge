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


#include "TerritoryConductorSpec.hpp"
#include "Texts.hpp"
#include "Building.hpp"
#include "ColorTheme.hpp"


TerritoryConductorSpec::TerritoryConductorSpec() = default;
std::vector<HorizontalSelectionWindowComponent> TerritoryConductorSpec::getComponents(const Building* building, MapState* state) {
	if (!building->works() and !this->conductsIfNotWork()) {
		HorizontalSelectionWindowComponent component = {
			"hammer_icon",
			*Texts::get()->get("does_not_expand_territory_if_hp_isnt_full"),
			false,
			Events()
		};
		return { component };
	}
	if (!building->connectedToOrigin(state)) {
		HorizontalSelectionWindowComponent component = {
			"road",
			*Texts::get()->get("does_not_lead_to_city_center"),
			false,
			Events()
		};
		return { component };
	}
	return {};
}
Events TerritoryConductorSpec::getHighlightEvent(const Building* building, MapState* state) {
	if (!building->connectedToOrigin(state) or (!building->works() and !this->conductsIfNotWork())) {
		return Events();
	}
	return this->AreaControllerSpec::getHighlightEvent(building, state);
}
bool TerritoryConductorSpec::allowBuilding(const Building *building, MapState* state, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2) {
	if (!building->connectedToOrigin(state) or (!building->works() and !this->conductsIfNotWork())) {
		return false;
	}
	return this->AreaControllerSpec::inRadius(building, state, x2, y2, sx2, sy2);
}
bool TerritoryConductorSpec::isActiveConductor(const Building *building) const {
	return this->conductsIfNotWork() or building->works();
}
sf::Color TerritoryConductorSpec::getHighlightColor() const {
    return COLOR_THEME::CELL_COLOR_HIGHLIGHTED_BLUE;
}