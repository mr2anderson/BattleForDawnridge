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
#include "ChangeHighlightEvent.hpp"


AreaB::AreaB() = default;
AreaB::AreaB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Building(x, y, sx, sy, maxHp, playerId, units) {

}
bool AreaB::inRadius(GO *go) const {
	return this->inRadius(go->getX(), go->getY(), go->getSX(), go->getSY());
}
bool AreaB::inRadius(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) const {
	sf::IntRect rect2;
	rect2.left = (int32_t)this->getX() - (int32_t)this->getRadius();
	rect2.top = (int32_t)this->getY() - (int32_t)this->getRadius();
	rect2.width = this->getSX() + 2 * this->getRadius() + 1;
	rect2.height = this->getSY() + 2 * this->getRadius() + 1 ;
	for (uint32_t i = x; i <= x + sx; i = i + sx) {
		for (uint32_t j = y; j <= y + sy; j = j + sy) {
			if (!rect2.contains(i, j)) {
				return false;
			}
		}
	}
	return true;
}
Events AreaB::getHighlightEvent() const {
	Events gEvent;
	if (!this->exist()) {
		return gEvent;
	}

	for (uint32_t x = this->getAreaXMin(); x <= this->getAreaXMax(); x = x + 1) {
		for (uint32_t y = this->getAreaYMin(); y <= this->getAreaYMax(); y = y + 1) {
			gEvent.add(std::make_shared<ChangeHighlightEvent>(this, x, y));
		}
	}

	return gEvent;
}
uint32_t AreaB::getAreaXMin() const {
	if (this->getX() >= this->getRadius()) {
		return this->getX() - this->getRadius();
	}
	return 0;
}
uint32_t AreaB::getAreaXMax() const {
	return this->getX() + this->getSX() - 1 + this->getRadius();
}
uint32_t AreaB::getAreaYMin() const {
	if (this->getY() >= this->getRadius()) {
		return this->getY() - this->getRadius();
	}
	return 0;
}
uint32_t AreaB::getAreaYMax() const {
	return this->getY() + this->getSY() - 1 + this->getRadius();
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