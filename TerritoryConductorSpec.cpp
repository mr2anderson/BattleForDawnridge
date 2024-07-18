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


TerritoryConductorSpec::TerritoryConductorSpec() = default;
std::vector<HorizontalSelectionWindowComponent> TerritoryConductorSpec::getComponents(MapState* state, uint32_t playerId, const std::string& soundName, bool works, bool connectedToOrigin) {
	if (!works and !this->conductsIfNotWork()) {
		HorizontalSelectionWindowComponent component = {
			"hammer_icon",
			*Texts::get()->get("does_not_expand_territory_if_hp_isnt_full"),
			false,
			Events()
		};
		return { component };
	}
	if (!connectedToOrigin) {
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
Events TerritoryConductorSpec::getHighlightEvent(MapState* state, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, bool works, bool connectedToOrigin) {
	if (!connectedToOrigin or (!works and !this->conductsIfNotWork())) {
		return Events();
	}
	return this->TerritoryExpanderSpec::getHighlightEvent(state, x, y, sx, sy, playerId, works, connectedToOrigin);
}
bool TerritoryConductorSpec::allowBuilding(MapState* state, uint32_t x1, uint32_t y1, uint32_t sx1, uint32_t sy1, uint32_t playerId, bool works, bool connectedToOrigin, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2) {
	if (!connectedToOrigin or (!works and !this->conductsIfNotWork())) {
		return false;
	}
	return this->TerritoryExpanderSpec::allowBuilding(state, x1, y1, sx1, sy1, playerId, works, connectedToOrigin, x2, y2, sx2, sy2);
}
bool TerritoryConductorSpec::isActiveConductor(bool works) const {
	return this->conductsIfNotWork() or works;
}