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
#include "Locales.hpp"



std::vector<BuildingHorizontalSelectionWindowComponent> IPopulationLimitIncreaserSpec::getComponents(std::shared_ptr<const Building>  building, MapState* state) {
	BuildingHorizontalSelectionWindowComponent component;

	if (building->wasWithFullHP()) {
		component = {
			HorizontalSelectionWindowComponent(  "helmet",
            StringLcl("{this_building_increases_population_limit}") + std::to_string(this->getPopulationLimit(building)),
			false,
			Events()),
            true
		};
	}
	else {
		component = {
			HorizontalSelectionWindowComponent(  "hammer_icon",
			StringLcl("{does_not_increase_population_limit_while_is_not_built_yet}"),
			false,
			Events()),
            true
		};
	}

	return { component };
}
uint32_t IPopulationLimitIncreaserSpec::getPopulationLimit(std::shared_ptr<const Building>  building) const {
	if (!building->wasWithFullHP()) {
		return 0;
	}
	return this->getActivePopulationLimit();
}
NewMoveMainPriority IPopulationLimitIncreaserSpec::getNewMoveMainPriority() const {
    return GO::NEW_MOVE_MAIN_PRIORITY_HOUSE;
}


BOOST_CLASS_EXPORT_IMPLEMENT(IPopulationLimitIncreaserSpec)