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


class WarriorHealerSpec : public AreaControllerSpec {
public:
	WarriorHealerSpec();

	Events getActiveNewMoveEvent(const Building* b, MapState* state) override;
	std::vector<BuildingHorizontalSelectionWindowComponent> getComponents(const Building* b, MapState* state) override;
	uint32_t getRadius() const override;
    sf::Color getHighlightColor(uint32_t playerId) const override;
    uint8_t getHighlightType() const override;
	virtual uint32_t getHealingSpeed() const = 0;
    virtual bool healVehicles() const;
    virtual std::string getHealTextureName() const = 0;
};