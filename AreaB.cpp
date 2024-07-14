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


#include "AreaB.hpp"
#include "Texts.hpp"


AreaB::AreaB() = default;
AreaB::AreaB(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Building(x, y, playerId, units) {

}
bool AreaB::inRadius(GO *go) {
	return this->inRadius(go->getX(), go->getY(), go->getSX(), go->getSY());
}
HorizontalSelectionWindowComponent AreaB::getExitComponent() {
	HorizontalSelectionWindowComponent component = this->Building::getExitComponent();
	component.gEvent = this->getHighlightEvent() + component.gEvent;
	return component;
}
HorizontalSelectionWindowComponent AreaB::getDestroyComponent() {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events destroyEvent = clickSoundEvent + this->getHighlightEvent();
    destroyEvent.add(std::make_shared<DestroyEvent>(this));
    std::shared_ptr<WindowTwoButtons> verify = std::make_shared<WindowTwoButtons>(*Texts::get()->get("verify_destroy"), *Texts::get()->get("yes"), *Texts::get()->get("no"), destroyEvent, clickSoundEvent + this->getHighlightEvent());
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