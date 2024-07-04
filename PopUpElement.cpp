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


#include "PopUpElement.hpp"


PopUpElement::PopUpElement() {
	this->_finished = false;
}
void PopUpElement::addOnStartGEvent(const GEvent& gEvent) {
	this->onStart = this->onStart + gEvent;
}
bool PopUpElement::finished() const {
	return this->_finished;
}
Events PopUpElement::run(uint32_t windowW, uint32_t windowH) {
	return Events(this->onStart, UIEvent());
}
void PopUpElement::finish() {
	this->_finished = true;
}