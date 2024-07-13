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


AreaB::AreaB() = default;
AreaB::AreaB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Building(x, y, sx, sy, maxHp, playerId, units) {

}
bool AreaB::inRadius(GO *go) {
	return this->inRadius(go->getX(), go->getY(), go->getSX(), go->getSY());
}
HorizontalSelectionWindowComponent AreaB::getExitComponent() const {
	HorizontalSelectionWindowComponent component = this->Building::getExitComponent();
	component.gEvent = component.gEvent + this->getHighlightEvent();
	return component;
}
HorizontalSelectionWindowComponent AreaB::getDestroyComponent() {
	HorizontalSelectionWindowComponent component = this->Building::getDestroyComponent();
	std::shared_ptr<CreateEEvent> createEvent = std::static_pointer_cast<CreateEEvent>(component.gEvent.at(0));
	std::shared_ptr<WindowTwoButtons> window = std::static_pointer_cast<WindowTwoButtons>(createEvent->getElement());
	window->addEvent1(this->getHighlightEvent());
	window->addEvent2(this->getHighlightEvent());
	return component;
}