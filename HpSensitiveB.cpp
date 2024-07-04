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
	return (this->getHP() == this->getMaxHP());
}
GOR HpSensitiveB::handleDoesNotWork() const {
	GOR response;
	MessageW* window = new MessageW(this->getNewWindowSoundName(), "click",
		this->getUpperCaseReadableName() + L": ÈÄÅÒ ÑÒÐÎÈÒÅËÜÑÒÂÎ\n"
		"Äîæäèòåñü êîíöà ñòðîèòåëüñòâà.\n"
		+ this->getReadableHpInfo() + L"\n"
		+ this->getReadableRegenerationSpeed());
	response.elements.push(window);
	return response;
}
GOR HpSensitiveB::regenerate() {
	bool works1 = this->works();
	GOR response = this->Building::regenerate();
	bool works2 = this->works();
	if (works2 and !works1) {
		FlyingE* element = new FlyingE("hammer_icon", this->getNewWindowSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
		response.elements.push(element);
	}
	return response;
}