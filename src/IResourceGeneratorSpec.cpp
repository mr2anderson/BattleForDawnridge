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


#include "IResourceGeneratorSpec.hpp"
#include "PlaySoundEvent.hpp"
#include "ImageFlyingE.hpp"
#include "AddResourceEvent.hpp"
#include "FocusOnEvent.hpp"
#include "CreateEEvent.hpp"
#include "Locales.hpp"
#include "Building.hpp"
#include "StaticString.hpp"


Events IResourceGeneratorSpec::getActiveNewMoveEvent(const Building *building, MapState* state) {
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

	if (limit.get(this->getProduct().type) == state->getPlayersPtr()->getPlayerPtr(building->getPlayerId())->getResource(this->getProduct().type)) {
		return Events();
	}

	Events event;

	std::shared_ptr<ImageFlyingE> flyingE = std::make_shared<ImageFlyingE>(this->getProduct().type + "_icon", building->getX(), building->getY(), building->getSX(), building->getSY());
	event.add(std::make_shared<FocusOnEvent>(building->getX(), building->getY(), building->getSX(), building->getSY()));
	event.add(std::make_shared<PlaySoundEvent>(this->getProduct().type));
	event.add(std::make_shared<CreateEEvent>(flyingE));
	event.add(std::make_shared<AddResourceEvent>(this->getProduct(), limit));

	return event;
}
std::vector<BuildingHorizontalSelectionWindowComponent> IResourceGeneratorSpec::getComponents(const Building *building, MapState* state) {
	BuildingHorizontalSelectionWindowComponent component;

	if (building->works()) {
		component = {
            HorizontalSelectionWindowComponent(
			std::make_shared<StaticString>(this->getProduct().type + "_icon"),
            StringLcl("{this_building_produces_resources_every_move}") + this->getProduct().getReadableInfo(),
			false,
			Events()),
            true
		};
	}
	else {
		component = {
			HorizontalSelectionWindowComponent(std::make_shared<StaticString>("hammer_icon"),
			StringLcl("{this_building_cant_produce_resources_if_hp_isnt_full}"),
			false,
			Events()),
            true
		};
	}

	return { component };
}


BOOST_CLASS_EXPORT_IMPLEMENT(IResourceGeneratorSpec)