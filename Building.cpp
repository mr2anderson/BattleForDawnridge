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


#include "Building.hpp"
#include "PlaySoundEvent.hpp"
#include "Texts.hpp"
#include "FlyingE.hpp"
#include "PlayerPointerCircle.hpp"


Building::Building() = default;
Building::Building(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Unit(x, y, sx, sy, 1, maxHp, playerId, units) {

}
bool Building::works() const {
	return this->exist();
}
Events Building::destroy() {
	this->subHp(this->getHP());
	Events soundEvent;
	soundEvent.add(std::make_shared<PlaySoundEvent>("destroy"));
	return soundEvent;
}
Events Building::regenerate() {
	Events events;
	if (this->getHP() < this->getMaxHP()) {
		std::shared_ptr<FlyingE> element = std::make_shared<FlyingE>("shield_icon", this->getX(), this->getY(), this->getSX(), this->getSY());
        events.add(std::make_shared<PlaySoundEvent>("regeneration"));
		events.add(std::make_shared<CreateEEvent>(element));
		events.add(std::make_shared<AddHpEvent>(this, this->getRegenerationSpeed()));
	}
	return events;
}
HorizontalSelectionWindowComponent Building::getHpInfoComponent() const {
	HorizontalSelectionWindowComponent component = {
		"shield_icon",
		*Texts::get()->get("hp") + std::to_wstring(this->getHP()) + L" / " + std::to_wstring(this->getMaxHP()) + L"\n" +
		*Texts::get()->get("building_speed") + std::to_wstring(this->getRegenerationSpeed()) + *Texts::get()->get("p_per_move"),
		false,
		Events()
	};
	return component;
}
HorizontalSelectionWindowComponent Building::getDestroyComponent() {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events destroyEvent = clickSoundEvent;
	destroyEvent.add(std::make_shared<DestroyEvent>(this));
	std::shared_ptr<WindowTwoButtons> verify = std::make_shared<WindowTwoButtons>(*Texts::get()->get("verify_destroy"), *Texts::get()->get("yes"), *Texts::get()->get("no"), destroyEvent, clickSoundEvent);
	Events createVerify = clickSoundEvent;
	createVerify.add(std::make_shared<CreateEEvent>(verify));
	HorizontalSelectionWindowComponent component = {
		"destroy_icon",
		*Texts::get()->get("destroy_this_building"),
		true,
		createVerify
	};
	return component;
}
bool Building::warriorCanStay(uint32_t warriorPlayerId) const {
	return true;
}
uint32_t Building::getWarriorMovementCost(uint32_t warriorPlayerId) const {
	return 1;
}
std::shared_ptr<PlayerPointer> Building::getPlayerPointer() const {
    return std::make_shared<PlayerPointerCircle>(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY());
}