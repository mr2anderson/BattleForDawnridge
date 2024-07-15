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


#include "VictoryConditionB.hpp"
#include "VictoryConditionBDestroyedEvent.hpp"
#include "Texts.hpp"
#include "ImageFlyingE.hpp"
#include "AddResourceEvent.hpp"
#include "CreateEEvent.hpp"


VictoryConditionB::VictoryConditionB() = default;
VictoryConditionB::VictoryConditionB(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Building(x, y, playerId, units) {

}
Events VictoryConditionB::destroy() {
	Events response = this->Building::destroy();
	response.add(std::make_shared<VictoryConditionBDestroyedEvent>(this->getPlayerId()));
	return response;
}
HorizontalSelectionWindowComponent VictoryConditionB::getVictoryConditionComponent() const {
	HorizontalSelectionWindowComponent component = {
		"star_icon",
		*Texts::get()->get("victory_condition_building_description"),
		false,
		Events()
	};
	return component;
}
Events VictoryConditionB::addWood() const {
	Events response;
	std::shared_ptr<ImageFlyingE> flyingE = std::make_shared<ImageFlyingE>("wood_icon", this->getX(), this->getY(), this->getSX(), this->getSY());
    response.add(std::make_shared<PlaySoundEvent>("wood"));
	response.add(std::make_shared<CreateEEvent>(flyingE));
	response.add(std::make_shared<AddResourceEvent>(Resource("wood", 1000)));
	return response;
}