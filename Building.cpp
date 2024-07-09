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


#include "Building.hpp"


Building::Building() = default;
Building::Building(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId) : 
	Unit(x, y, sx, sy, 1, maxHp, playerId) {
}
bool Building::works() const {
	return this->exist();
}
Events Building::regenerate() {
	Events events;
	if (this->getHP() < this->getMaxHP()) {
		Events gEvent;
		gEvent.add(std::make_shared<AddHpEvent>(this, this->getRegenerationSpeed()));
		std::shared_ptr<FlyingE> element = std::make_shared<FlyingE>("shield_icon", "regeneration", this->getX(), this->getY(), this->getSX(), this->getSY());
		element->addOnStartGEvent(gEvent);
		events.add(std::make_shared<CreateEEvent>(element));
	}
	return events;
}
HorizontalSelectionWindowComponent Building::getHpInfoComponent() const {
	HorizontalSelectionWindowComponent component = {
		"shield_icon",
		*Texts::get()->get("hp") + std::to_wstring(this->getHP()) + L" / " + std::to_wstring(this->getMaxHP()) + L"\n" +
		*Texts::get()->get("building_speed") + std::to_wstring(this->getRegenerationSpeed()) + *Texts::get()->get("p_per_move"),
		false,
		false,
		Events()
	};
	return component;
}