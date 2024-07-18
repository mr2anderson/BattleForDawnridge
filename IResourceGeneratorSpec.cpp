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
#include "Texts.hpp"


Events IResourceGeneratorSpec::getActiveNewMoveEvent(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, MapState* state, uint32_t playerId, const std::string &soundName, bool works) {
	if (!works) {
		return Events();
	}

	Events event;

	std::shared_ptr<ImageFlyingE> flyingE = std::make_shared<ImageFlyingE>(this->getProduct().type + "_icon", x, y, sx, sy);
	event.add(std::make_shared<FocusOnEvent>(x, y, sx, sy));
	event.add(std::make_shared<PlaySoundEvent>(this->getProduct().type));
	event.add(std::make_shared<CreateEEvent>(flyingE));
	event.add(std::make_shared<AddResourceEvent>(this->getProduct()));

	return event;
}
std::vector<HorizontalSelectionWindowComponent> IResourceGeneratorSpec::getComponents(MapState* state, uint32_t playerId, const std::string &soundName, bool works, bool connectedToOrigin) {
	HorizontalSelectionWindowComponent component;

	if (works) {
		component = {
			this->getProduct().type + "_icon",
			*Texts::get()->get("this_building_produces_resources_every_move") + this->getProduct().getReadableInfo(),
			false,
			Events()
		};
	}
	else {
		component = {
			"hammer_icon",
			*Texts::get()->get("this_building_cant_produce_resources_if_hp_isnt_full"),
			false,
			Events()
		};
	}

	return { component };
}