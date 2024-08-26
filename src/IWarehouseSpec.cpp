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


#include "IWarehouseSpec.hpp"
#include "Locales.hpp"
#include "Building.hpp"
#include "LimitResourcesEvent.hpp"



Events IWarehouseSpec::getEventOnDestroy(std::shared_ptr<const Building> building, MapState *state) const {
	Events event;

	Resources newLimit;
	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
        std::shared_ptr<Building> b = state->getCollectionsPtr()->getBuilding(i);
        if (b->exist() and b->getPlayerId() == building->getPlayerId() and b->getUUID() != building->getUUID()) {
            newLimit.plus(b->getLimit());
        }
    }

    event.add(std::make_shared<LimitResourcesEvent>(building->getPlayerId(), newLimit));

	return event;
}
std::vector<BuildingHorizontalSelectionWindowComponent> IWarehouseSpec::getComponents(std::shared_ptr<const Building> building, MapState* state) {
	BuildingHorizontalSelectionWindowComponent component;

    if (building->wasWithFullHP()) {
        component = {HorizontalSelectionWindowComponent(
                          "resources_icon",
                        StringLcl("{resource_storage_building_description}") + this->getActiveLimit().getReadableInfo(),
                        false,
                        Events()),
                true
        };
    }
    else {
        component = {
                HorizontalSelectionWindowComponent(
                          "hammer_icon",
                        StringLcl("{does_not_increase_resource_limit_if_isnt_built_yet}"),
                        false,
                        Events()
                        ),
                        true
        };
    }

	return { component };
}
NewMoveMainPriority IWarehouseSpec::getNewMoveMainPriority() const {
    return GO::NEW_MOVE_MAIN_PRIORITY_WAREHOUSE;
}
Resources IWarehouseSpec::getLimit(std::shared_ptr<const Building> building) const {
    if (building->wasWithFullHP()) {
        return this->getActiveLimit();
    }
    return {};
}


BOOST_CLASS_EXPORT_IMPLEMENT(IWarehouseSpec)