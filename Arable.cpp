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


#include "Arable.hpp"


Arable::Arable() = default;
Arable::Arable(uint32_t x, uint32_t y, uint32_t playerId) :
	HpSensitiveB(x, y, 3, 3, 2000, playerId),
	Building(x, y, 3, 3, 2000, playerId) {

}
Building* Arable::cloneBuilding() const {
	return new Arable(*this);
}
Events Arable::newMove(uint32_t playerId) {
	if (this->exist() and this->belongTo(playerId)) {
		this->collected = false;
		return this->regenerate();
	}
	return Events();
}
Resources Arable::getCost() const {
	return Resources({ Resource("wood", 4000)});
}
std::string Arable::getTextureName() const {
	return "arable";
}
std::string Arable::getSoundName() const {
	return "food";
}
std::wstring Arable::getDescription() const {
	return *Texts::get()->get("arable_description");
}
bool Arable::alreadyCollected() const {
	return this->collected;
}
uint32_t Arable::collect() const {
	return 1000;
}
uint32_t Arable::getRegenerationSpeed() const {
	return 1000;
}
std::wstring Arable::getUpperCaseReadableName() const {
	return *Texts::get()->get("arable_upper_case_readable_name");
}
Events Arable::getSelectionW() {
	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	components.push_back(this->getDestroyComponent());
	if (this->repairing()) {
		components.push_back(this->getBusyWithRepairingComponent());
	}

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(this->getSoundName(), "click", components);
	Events windowEvent;
	windowEvent.add(std::make_shared<CreateEEvent>(window));
	return windowEvent;
}
Events Arable::getGameObjectResponse(uint32_t playerId) {
	if (this->exist()) {
		if (this->belongTo(playerId)) {
			return this->getSelectionW();
		}
		return this->getUnitOfEnemyResponse();
	}
	return Events();
}