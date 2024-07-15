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
#include "Texts.hpp"
#include "ImageFlyingE.hpp"
#include "AddResourceEvent.hpp"
#include "CreateEEvent.hpp"


Arable::Arable() = default;
Arable::Arable(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	HpSensitiveB(x, y, playerId, units),
	Building(x, y, playerId, units) {

}
Building* Arable::cloneBuilding() const {
	return new Arable(*this);
}
Events Arable::newMove(uint32_t playerId) {
	if (this->exist() and this->belongTo(playerId)) {
		Events e = this->regenerate();
		if (this->works()) {
			e = e + this->addFood();
		}
		return e;
	}
	return Events();
}
uint32_t Arable::getSX() const {
    return 2;
}
uint32_t Arable::getSY() const {
    return 2;
}
uint32_t Arable::getMaxHP() const {
	return 1000;
}
Defence Arable::getDefence() const {
	return Defence::WOOD;
}
Resources Arable::getCost() const {
	return Resources({ Resource("wood", 5000)});
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
uint32_t Arable::getRegenerationSpeed() const {
	return this->getMaxHP() / 2;
}
std::wstring Arable::getUpperCaseReadableName() const {
	return *Texts::get()->get("arable_upper_case_readable_name");
}
Events Arable::addFood() const {
	Events response;
	std::shared_ptr<ImageFlyingE> flyingE = std::make_shared<ImageFlyingE>("food_icon", this->getX(), this->getY(), this->getSX(), this->getSY());
    response.add(std::make_shared<PlaySoundEvent>("click"));
	response.add(std::make_shared<CreateEEvent>(flyingE));
	response.add(std::make_shared<AddResourceEvent>(Resource("food", 875)));
	return response;
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

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);
	Events windowEvent;
    windowEvent.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	windowEvent.add(std::make_shared<CreateEEvent>(window));
	return windowEvent;
}
Events Arable::getGameObjectResponse(uint32_t playerId) {
	if (this->exist()) {
		if (this->belongTo(playerId)) {
			return this->getSelectionW();
		}
		return Events();
	}
	return Events();
}