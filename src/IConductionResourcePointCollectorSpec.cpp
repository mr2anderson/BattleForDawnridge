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


#include "IConductionResourcePointCollectorSpec.hpp"
#include "ConductionResourcePoint.hpp"
#include "Building.hpp"
#include "ImageFlyingE.hpp"
#include "FocusOnEvent.hpp"
#include "CreateEEvent.hpp"
#include "AddResourceEvent.hpp"
#include "Locales.hpp"
#include "SubHpEvent.hpp"

#include "PlaySoundEvent.hpp"
#include "ColorTheme.hpp"


Events IConductionResourcePointCollectorSpec::getActiveNewMoveEvent(std::shared_ptr<const Building>  building, MapState* state) {
	if (!building->works()) {
		return Events();
	}

	Resources limit;
	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		std::shared_ptr<Building>  b = state->getCollectionsPtr()->getBuilding(i);
		if (b->exist() and b->getPlayerId() == building->getPlayerId()) {
			limit.plus(b->getLimit());
		}
	}

	uint32_t leftSpace = limit.get(this->getResourceType()) - state->getPlayersPtr()->getPlayerPtr(building->getPlayerId())->getResource(this->getResourceType());

	std::vector<std::tuple<std::shared_ptr<ResourcePoint>, uint32_t>> src;

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalConductionRPs(); i = i + 1) {
		std::shared_ptr<ConductionResourcePoint> rp = state->getCollectionsPtr()->getConductionRP(i);
		if (rp->exist() and rp->getResourceType() == this->getResourceType() and building->connectedTo(state, rp)) {
			uint32_t got = rp->tryToCollect(building->getPlayerId(), std::min(leftSpace, this->getCollectionSpeed()));
			if (got != 0) {
				leftSpace = leftSpace - got;
				src.emplace_back(rp, got);
			}
		}
	}

	if (src.empty()) {
		return Events();
	}

	std::shared_ptr<ImageFlyingE> flyingE = std::make_shared<ImageFlyingE>(this->getResourceType() + "_icon", building->getX(), building->getY(), building->getSX(), building->getSY());

	Events response;
	response.add(std::make_shared<FocusOnEvent>(building->getX(), building->getY(), building->getSX(), building->getSY()));
	response.add(std::make_shared<PlaySoundEvent>(this->getResourceType()));
	response.add(std::make_shared<CreateEEvent>(flyingE));
	for (uint32_t i = 0; i < src.size(); i = i + 1) {
		response.add(std::make_shared<AddResourceEvent>(Resource(this->getResourceType(), std::get<uint32_t>(src.at(i))), limit));
		response.add(std::make_shared<SubHpEvent>(std::get<std::shared_ptr<ResourcePoint>>(src.at(i)), std::get<uint32_t>(src.at(i))));
	}

	return response;
}
std::vector<BuildingHorizontalSelectionWindowComponent> IConductionResourcePointCollectorSpec::getComponents(std::shared_ptr<const Building>  building, MapState* state) {
	BuildingHorizontalSelectionWindowComponent component;

	if (building->works()) {
		uint32_t n = 0;
		for (uint32_t i = 0; i < state->getCollectionsPtr()->totalConductionRPs(); i = i + 1) {
			std::shared_ptr<ConductionResourcePoint> rp = state->getCollectionsPtr()->getConductionRP(i);
			n = n + (rp->exist() and rp->getResourceType() == this->getResourceType() and building->connectedTo(state, rp));
		}

		StringLcl s;
		if (n == 0) {
			s = StringLcl("{no_conducted_resource_points}");
		}
		else {
			s = StringLcl("{conducted_resource_points_number}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + StringLcl(std::to_string(n));
		}

		component = {
			HorizontalSelectionWindowComponent(this->getResourceType() + "_icon",
            StringLcl("{this_building_collects_resources_from_conducted_resource_points}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + StringLcl(std::to_string(this->getCollectionSpeed())) + StringLcl(". ") + s,
			false,
			Events()),
            true
		};
	}
	else {
		component = {
			HorizontalSelectionWindowComponent(  "hammer_icon",
			StringLcl("{this_building_cant_collect_resources_if_hp_isnt_full}"),
			false,
			Events()),
            true
		};
	}

	return { component };
}


BOOST_CLASS_EXPORT_IMPLEMENT(IConductionResourcePointCollectorSpec)