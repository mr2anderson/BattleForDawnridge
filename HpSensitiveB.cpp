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


#include "HpSensitiveB.hpp"
#include "Texts.hpp"
#include "ImageFlyingE.hpp"


HpSensitiveB::HpSensitiveB() = default;
HpSensitiveB::HpSensitiveB(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Building(x, y, playerId, units) {
}
bool HpSensitiveB::works() const {
	return this->Building::works() and !this->repairing();
}
bool HpSensitiveB::repairing() const {
	return (this->getHP() < this->getMaxHP());
}
HorizontalSelectionWindowComponent HpSensitiveB::getBusyWithRepairingComponent() const {
	HorizontalSelectionWindowComponent component = {
		"hammer_icon",
		*Texts::get()->get("building_in_progress"),
		false,
		Events()
	};
	return component;
}
Events HpSensitiveB::regenerate() {
	Events events = this->Building::regenerate();
	if (this->getHP() != this->getMaxHP() and this->getHP() + this->getRegenerationSpeed() >= this->getMaxHP()) {
		std::shared_ptr<ImageFlyingE> element = std::make_shared<ImageFlyingE>("hammer_icon", this->getX(), this->getY(), this->getSX(), this->getSY());
        events.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
		events.add(std::make_shared<CreateEEvent>(element));
	}
	return events;
}