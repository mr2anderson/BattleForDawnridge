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


#include "Building.hpp"


Building::Building() = default;
Building::Building(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, const Player* playerPtr) : 
	Unit(x, y, sx, sy, 1, maxHp, playerPtr) {

}
std::wstring Building::getUpperCaseReadableName() const {
	std::wstring s = this->getReadableName();
	std::wstring n;
	for (auto c : s) {
		switch (c) {
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		case L'�': { n += L'�'; break; }
		default: { n += c; break; }
		}
	}
	return n;
}
GOR Building::regenerate() {
	GOR responce;
	if (this->getHP() < this->getMaxHP()) {
		GEvent gEvent;
		gEvent.addHp.emplace_back(this, this->getRegenerationSpeed());
		FlyingE* element = new FlyingE("shield_icon", "regeneration", this->getX(), this->getY(), this->getSX(), this->getSY());
		element->addOnStartGEvent(gEvent);
		responce.elements.push(element);
	}
	return responce;
}
std::wstring Building::getReadableRegenerationSpeed() const {
	return L"�������� �������������: " + std::to_wstring(this->getRegenerationSpeed()) + L" ��. / ���";
}