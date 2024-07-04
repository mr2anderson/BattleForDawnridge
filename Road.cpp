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


#include "Road.hpp"


Road::Road() = default;
Road::Road(uint32_t x, uint32_t y, const Player* playerPtr) :
	Building(x, y, 1, 1, 1000, playerPtr) {

}
GOR Road::newMove(const Player& player) {
	if (this->belongTo(&player) and this->exist()) {
		return this->regenerate();
	}
	return GOR();
}
Resources Road::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 1000));
	return cost;
}
uint32_t Road::getRegenerationSpeed() const {
	return 1000;
}
std::string Road::getTextureName() const {
	return "road";
}
std::string Road::getNewWindowSoundName() const {
	return "road";
}
std::wstring Road::getReadableName() const {
	return L"������";
}
GOR Road::getSelectionW() {
	GOR response;

	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", L"��������", true, true, GEvent());
	components.emplace_back("road",
		L"������ � ���� �����������. ��������� ������, ����� ����� ����������� ��������� ������ � ����� ������.\n"
		+ this->getReadableHpInfo(), false, false, GEvent());

	SelectionW* window = new SelectionW(this->getNewWindowSoundName(), "click", components);
	response.elements.push(window);

	return response;
}
GOR Road::getGameObjectResponse(const Player& player) {
	if (this->belongTo(&player)) {
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}