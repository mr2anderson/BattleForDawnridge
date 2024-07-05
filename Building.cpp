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
bool Building::works() const {
	return this->exist();
}
std::wstring Building::getUpperCaseReadableName() const {
	std::wstring s = this->getReadableName();
	std::wstring n;
	for (auto c : s) {
		n += TO_UPPER(c);
	}
	return n;
}
wchar_t Building::TO_UPPER(wchar_t a) {
	switch (a) {
	case L'й': { return L'Й';}
	case L'ц': {return L'Ц';}
	case L'у': {return L'У';}
	case L'к': {return L'К';}
	case L'е': {return L'Е';}
	case L'н': {return L'Н';}
	case L'г': {return L'Г';}
	case L'ш': {return L'Ш';}
	case L'щ': {return L'Щ';}
	case L'з': {return L'З'; }
	case L'х': {return L'Х';}
	case L'ъ': {return L'Ъ';}
	case L'ф': {return L'Ф';}
	case L'ы': {return L'Ы';}
	case L'в': {return L'В';}
	case L'а': {return L'А';}
	case L'п': {return L'П';}
	case L'р': {return L'Р';}
	case L'о': {return L'О';}
	case L'л': {return L'Л';}
	case L'д': {return L'Д';}
	case L'ж': {return L'Ж';}
	case L'э': {return L'Э';}
	case L'я': {return L'Я';}
	case L'ч': {return L'Ч';}
	case L'с': {return L'С';}
	case L'м': {return L'М';}
	case L'и': {return L'И';}
	case L'т': {return L'Т';}
	case L'ь': {return L'Ь';}
	case L'б': {return L'Б';}
	case L'ю': {return L'Ю';}
	case L'ё': {return L'Ё';}
	default: {return a;}
	}
}
Events Building::regenerate() {
	Events events;
	if (this->getHP() < this->getMaxHP()) {
		GEvent gEvent;
		gEvent.addHp.emplace_back(this, this->getRegenerationSpeed());
		FlyingE* element = new FlyingE("shield_icon", "regeneration", this->getX(), this->getY(), this->getSX(), this->getSY());
		element->addOnStartGEvent(gEvent);
		events.uiEvent.createE.push_back(element);
	}
	return events;
}
std::wstring Building::getReadableRegenerationSpeed() const {
	return L"Скорость строительства: " + std::to_wstring(this->getRegenerationSpeed()) + L" ед. / ход";
}