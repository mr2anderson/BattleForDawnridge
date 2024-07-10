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


#include "ResourceStorageB.hpp"
#include "Texts.hpp"
#include "ResourceStorageBDestroyedEvent.hpp"


ResourceStorageB::ResourceStorageB() = default;
ResourceStorageB::ResourceStorageB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId) :
	Building(x, y, sx, sy, maxHp, playerId) {

}
Events ResourceStorageB::destroy() {
	Events response = this->Building::destroy();
	response.add(std::make_shared<ResourceStorageBDestroyedEvent>(this->getPlayerId()));
	return response;
}
HorizontalSelectionWindowComponent ResourceStorageB::getResourceStorageComponent() const {
	HorizontalSelectionWindowComponent component = {
		"resources_icon",
		*Texts::get()->get("resource_storage_building_description") + this->getLimit().getReadableInfo(),
		false,
		false,
		Events()
	};
	return component;
}