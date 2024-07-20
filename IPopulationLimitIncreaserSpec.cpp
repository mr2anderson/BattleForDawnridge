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


#include "IPopulationLimitIncreaserSpec.hpp"
#include "Building.hpp"
#include "Texts.hpp"


std::vector<HorizontalSelectionWindowComponent> IPopulationLimitIncreaserSpec::getComponents(const Building* building, MapState* state) {
	HorizontalSelectionWindowComponent component;

	if (building->works()) {
		component = {
			"helmet",
			*Texts::get()->get("this_building_increases_population_limit") + std::to_wstring(this->getPopulationLimit(building)),
			false,
			Events()
		};
	}
	else {
		component = {
			"hammer_icon",
			*Texts::get()->get("does_not_increase_population_limit_if_hp_isnt_full"),
			false,
			Events()
		};
	}

	return { component };
}
uint32_t IPopulationLimitIncreaserSpec::getPopulationLimit(const Building* building) const {
	if (!building->works()) {
		return 0;
	}
	return this->getActivePopulationLimit();
}