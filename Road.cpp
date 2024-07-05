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
Road::Road(uint32_t x, uint32_t y, const Player* playerPtr, std::vector<TerritoryOriginB*>* originsPtr, std::vector<TerritoryConductorB*>* conductorsPtr) :
	TerritoryConductorB(x, y, 1, 1, 1000, playerPtr, originsPtr, conductorsPtr),
	Building(x, y, 1, 1, 1000, playerPtr) {
	
}
Event Road::newMove(const Player& player) {
	if (this->belongTo(&player) and this->exist()) {
		return this->regenerate();
	}
	return Event();
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
std::string Road::getSoundName() const {
	return "road";
}
std::wstring Road::getDescription() const {
	return L"ƒороги Ч вены цивилизации. ѕостройте дорогу, чтобы иметь возможность возводить здани€ в новых местах.";
}
std::wstring Road::getReadableName() const {
	return L"дорога";
}
uint32_t Road::getRadius() const {
	return 3;
}
Event Road::getSelectionW() {
	Event response;

	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", L"ѕокинуть", true, true, this->getHighlightEvent());
	components.emplace_back("road",
		this->getDescription() + L"\n"
		+ this->getReadableHpInfo(), false, false, Event());

	SelectionW* window = new SelectionW(this->getSoundName(), "click", components);
	response.createE.push_back(window);

	return response;
}
Event Road::getGameObjectResponse(const Player& player) {
	if (!this->exist()) {
		return Event();
	}
	if (this->belongTo(&player)) {
		if (this->repairing()) {
			return this->handleRepairing();
		}
		if (!this->conducted()) {
			return this->getNotConductedResponce();
		}
		Event responce;
		responce = this->getHighlightEvent();
		return responce + this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}