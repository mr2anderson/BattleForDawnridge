/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "HpSensitiveB.hpp"


HpSensitiveB::HpSensitiveB() = default;
HpSensitiveB::HpSensitiveB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, const Player* playerPtr) : 
	Building(x, y, sx, sy, maxHp, playerPtr) {
}
bool HpSensitiveB::works() const {
	return this->Building::works() and !this->repairing();
}
bool HpSensitiveB::repairing() const {
	return (this->getHP() < this->getMaxHP());
}
Events HpSensitiveB::handleRepairing() const {
	Events events;
	MessageW* window = new MessageW(this->getSoundName(), "click",
		this->getUpperCaseReadableName() + L": ÈÄÅÒ ÑÒÐÎÈÒÅËÜÑÒÂÎ\n"
		"Äîæäèòåñü êîíöà ñòðîèòåëüñòâà.\n"
		+ this->getReadableHpInfo() + L"\n"
		+ this->getReadableRegenerationSpeed());
	events.uiEvent.createE.push_back(window);
	return events;
}
Events HpSensitiveB::regenerate() {
	Events events = this->Building::regenerate();
	if (this->getHP() != this->getMaxHP() and this->getHP() + this->getRegenerationSpeed() >= this->getMaxHP()) {
		FlyingE* element = new FlyingE("hammer_icon", this->getSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
		events.uiEvent.createE.push_back(element);
	}
	return events;
}