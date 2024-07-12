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
#include "ColorTheme.hpp"


AreaB::AreaB() = default;
AreaB::AreaB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Building(x, y, sx, sy, maxHp, playerId, units) {

}
bool AreaB::inRadius(GO *go) const {
	return this->inRadius(go->getX(), go->getY(), go->getSX(), go->getSY());
}
bool AreaB::inRadius(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) const {
	sf::IntRect rect2;

	rect2.left = this->getAreaXMin();
	rect2.top = this->getAreaYMin();
	rect2.width = this->getAreaWidth() + 1;
	rect2.height = this->getAreaHeight() + 1;

	if (rect2.left != 0) {
		rect2.left = rect2.left - 1;
		rect2.width = rect2.width + 1;
	}
	if (rect2.top != 0) {
		rect2.top = rect2.top - 1;
		rect2.height = rect2.height + 1;
	}

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

	gEvent.add(std::make_shared<ChangeHighlightEvent>(this, COLOR_THEME::CELL_COLOR_HIGHLIGHTED_DARK, this->getAreaXMin(), this->getAreaYMin(), this->getAreaWidth(), this->getAreaHeight()));

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
uint32_t AreaB::getAreaWidth() const {
	return (this->getSX() + this->getRadius() + this->getX()) - this->getAreaXMin();
}
uint32_t AreaB::getAreaHeight() const {
	return (this->getSY() + this->getRadius() + this->getY()) - this->getAreaYMin();
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