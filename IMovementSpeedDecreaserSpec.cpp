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


#include "IMovementSpeedDecreaserSpec.hpp"
#include "Building.hpp"
#include "Texts.hpp"


std::vector<HorizontalSelectionWindowComponent> IMovementSpeedDecreaser::getComponents(const Building* building, MapState* state) {
	HorizontalSelectionWindowComponent component = {
		building->getTextureName(),
		*Texts::get()->get("this_building_decreases_movement_speed") + std::to_wstring(this->getMovementSpeedDecreasingEffectStrength()),
		false,
		Events()
	};

	return { component };
}
uint32_t IMovementSpeedDecreaser::getWarriorMovementCost(const Building* building, uint32_t playerId) const {
	return this->getMovementSpeedDecreasingEffectStrength();
}