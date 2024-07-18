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


#include "AreaResourcePointCollectorSpec.hpp"
#include "ResourcePoint.hpp"
#include "TryToCollectEvent.hpp"
#include "Texts.hpp"
#include "Building.hpp"


AreaResourcePointCollectorSpec::AreaResourcePointCollectorSpec() = default;
Events AreaResourcePointCollectorSpec::getActiveNewMoveEvent(const Building *building, MapState* state) {
	if (!building->works()) {
		return Events();
	}

	Events responce;

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalRPs(); i = i + 1) {
		ResourcePoint* rp = state->getCollectionsPtr()->getRP(i);
		if (rp->exist() and this->inRadius(building, state, rp->getX(), rp->getY(), rp->getSX(), rp->getSY()) and rp->getResourceType() == this->getResourceType()) {
			responce.add(std::make_shared<TryToCollectEvent>(building->getPlayerId(), rp, this->getCollectionSpeed()));
			break;
		}
	}

	return responce;
}
std::vector<HorizontalSelectionWindowComponent> AreaResourcePointCollectorSpec::getComponents(const Building *building, MapState* state) {
	HorizontalSelectionWindowComponent component;

	if (building->works()) {
		component = {
			this->getResourceType() + "_icon",
			*Texts::get()->get("this_building_collects_resources_from_resource_points") + std::to_wstring(this->getCollectionSpeed()),
			false,
			Events()
		};
	}
	else {
		component = {
			"hammer_icon",
			*Texts::get()->get("this_building_cant_collect_resources_if_hp_isnt_full"),
			false,
			Events()
		};
	}

	return { component };
}
uint32_t AreaResourcePointCollectorSpec::getRadius() const {
	return this->getCollectionRadius();
}