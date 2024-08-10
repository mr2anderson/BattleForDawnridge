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


#include "IAreaResourcePointCollectorSpec.hpp"
#include "AreaResourcePoint.hpp"
#include "Locales.hpp"
#include "Building.hpp"
#include "FocusOnEvent.hpp"
#include "ImageFlyingE.hpp"
#include "CreateEEvent.hpp"
#include "SubHpEvent.hpp"
#include "AddResourceEvent.hpp"
#include "HighlightColors.hpp"

#include "PlaySoundEvent.hpp"


Events IAreaResourcePointCollectorSpec::getActiveNewMoveEvent(const Building *building, MapState* state) {
	if (!building->works()) {
		return Events();
	}

	Resources limit;
	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		Building* b = state->getCollectionsPtr()->getBuilding(i);
		if (b->exist() and b->getPlayerId() == building->getPlayerId()) {
			limit.plus(b->getLimit());
		}
	}

	uint32_t leftSpace = limit.get(this->getResourceType()) - state->getPlayersPtr()->getPlayerPtr(building->getPlayerId())->getResource(this->getResourceType());

	uint32_t left = std::min(this->getCollectionSpeed(), leftSpace);

	std::vector<std::tuple<ResourcePoint*, uint32_t>> src;

	HashTableMapPosition<uint32_t> available = this->getAvailable(building->getX(), building->getY(), building->getSX(), building->getSY(), building->getPlayerId(), state);
	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalAreaRPs(); i = i + 1) {
		AreaResourcePoint* rp = state->getCollectionsPtr()->getAreaRP(i);
		if (rp->exist() and IAreaControllerSpec::IN_RADIUS(available, rp, IAreaControllerSpec::IN_RADIUS_TYPE::FULLY) and rp->getResourceType() == this->getResourceType()) {
			uint32_t got = rp->tryToCollect(building->getPlayerId(), left);
			if (got != 0) {
				src.emplace_back(rp, got);
				left = left - got;
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
		response.add(std::make_shared<SubHpEvent>(std::get<ResourcePoint*>(src.at(i)), std::get<uint32_t>(src.at(i))));
		response.add(std::make_shared<AddResourceEvent>(Resource(this->getResourceType(), std::get<uint32_t>(src.at(i))), limit));
	}

	return response;
}
std::vector<BuildingHorizontalSelectionWindowComponent> IAreaResourcePointCollectorSpec::getComponents(const Building *building, MapState* state) {
	std::vector<BuildingHorizontalSelectionWindowComponent> components;

	if (building->works()) {
		components.emplace_back(
			HorizontalSelectionWindowComponent(this->getResourceType() + "_icon",
            StringLcl("{this_building_collects_resources_from_resource_points}") + StringLcl(std::to_string(this->getCollectionSpeed())) + StringLcl(". ") +
            StringLcl("{resource_in_radius}") + StringLcl(std::to_string(this->countResourceInRadius(building, state))),
			false,
			Events()),
            true
        );
	}
	else {
		components.emplace_back(
			HorizontalSelectionWindowComponent(  "hammer_icon",
			StringLcl("{this_building_cant_collect_resources_if_hp_isnt_full}"),
			false,
			Events()),
            true
        );
	}

	return components;
}
uint32_t IAreaResourcePointCollectorSpec::getRadius() const {
	return this->getCollectionRadius();
}
sf::Color IAreaResourcePointCollectorSpec::getHighlightColor(uint32_t playerId) const {
    return HighlightColors::get()->getResourceCollectingColor(playerId);
}
uint8_t IAreaResourcePointCollectorSpec::getHighlightType() const {
    return IAreaControllerSpec::OTHER;
}
uint32_t IAreaResourcePointCollectorSpec::countResourceInRadius(const Building *building, MapState *state) {
    uint32_t ctr = 0;

	HashTableMapPosition<uint32_t> available = this->getAvailable(building->getX(), building->getY(), building->getSX(), building->getSY(), building->getPlayerId(), state);
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalAreaRPs(); i = i + 1) {
        AreaResourcePoint* rp = state->getCollectionsPtr()->getAreaRP(i);
        if (rp->exist() and IAreaControllerSpec::IN_RADIUS(available, rp, IAreaControllerSpec::IN_RADIUS_TYPE::FULLY) and rp->getResourceType() == this->getResourceType()) {
            ctr = ctr + rp->getHP();
        }
    }

    return ctr;
}


BOOST_CLASS_EXPORT_IMPLEMENT(IAreaResourcePointCollectorSpec)