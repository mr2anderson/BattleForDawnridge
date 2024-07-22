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


#include "AreaControllerSpec.hpp"


#pragma once


class TerritoryConductorSpec : public AreaControllerSpec {
public:
	TerritoryConductorSpec();

	std::vector<HorizontalSelectionWindowComponent> getComponents(const Building* building, MapState* state) override;
	Events getHighlightEvent(const Building* building, MapState* state, uint8_t) override;
	bool allowBuilding(const Building* building, MapState* state, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2) override;
	bool isActiveConductor(const Building* building) const override;
    sf::Color getHighlightColor() const override;
    uint8_t getHighlightType() const override;
	virtual bool conductsIfNotWork() const = 0;
};