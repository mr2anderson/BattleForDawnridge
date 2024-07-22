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


#include "TerritoryOriginSpec.hpp"
#include "ColorTheme.hpp"


TerritoryOriginSpec::TerritoryOriginSpec() = default;
bool TerritoryOriginSpec::allowBuilding(const Building* building, MapState* state, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2) {
	return this->AreaControllerSpec::inRadius(building, state, x2, y2, sx2, sy2);
}
bool TerritoryOriginSpec::isOrigin() const {
	return true;
}
sf::Color TerritoryOriginSpec::getHighlightColor() const {
    return COLOR_THEME::CELL_COLOR_HIGHLIGHTED_BLUE;
}
uint8_t TerritoryOriginSpec::getHighlightType() const {
    return AreaControllerSpec::HIGHLIGHT_TYPE::TERRITORY;
}