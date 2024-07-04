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
		case L'й': { n += L'Й'; break; }
		case L'ц': { n += L'Ц'; break; }
		case L'у': { n += L'У'; break; }
		case L'к': { n += L'К'; break; }
		case L'е': { n += L'Е'; break; }
		case L'н': { n += L'Н'; break; }
		case L'г': { n += L'Г'; break; }
		case L'ш': { n += L'Ш'; break; }
		case L'щ': { n += L'Щ'; break; }
		case L'з': { n += L'З'; break; }
		case L'х': { n += L'Х'; break; }
		case L'ъ': { n += L'Ъ'; break; }
		case L'ф': { n += L'Ф'; break; }
		case L'ы': { n += L'Ы'; break; }
		case L'в': { n += L'В'; break; }
		case L'а': { n += L'А'; break; }
		case L'п': { n += L'П'; break; }
		case L'р': { n += L'Р'; break; }
		case L'о': { n += L'О'; break; }
		case L'л': { n += L'Л'; break; }
		case L'д': { n += L'Д'; break; }
		case L'ж': { n += L'Ж'; break; }
		case L'э': { n += L'Э'; break; }
		case L'я': { n += L'Я'; break; }
		case L'ч': { n += L'Ч'; break; }
		case L'с': { n += L'С'; break; }
		case L'м': { n += L'М'; break; }
		case L'и': { n += L'И'; break; }
		case L'т': { n += L'Т'; break; }
		case L'ь': { n += L'Ь'; break; }
		case L'б': { n += L'Б'; break; }
		case L'ю': { n += L'Ю'; break; }
		case L'ё': { n += L'Ё'; break; }
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
	return L"Скорость строительства: " + std::to_wstring(this->getRegenerationSpeed()) + L" ед. / ход";
}