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


#include "PopUpElement.hpp"
#include "PlaySoundEvent.hpp"


PopUpElement::PopUpElement() {
	this->_finished = false;
}
bool PopUpElement::finished() const {
	return this->_finished;
}
void PopUpElement::restart() {
	this->_finished = false;
	this->onRestart();
}
void PopUpElement::update() {

}
Events PopUpElement::click(uint32_t mouseX, uint32_t mouseY, uint32_t windowW, uint32_t windowH) {
	return Events();
}
void PopUpElement::onRestart() {

}
void PopUpElement::finish() {
	this->_finished = true;
}


BOOST_CLASS_EXPORT_IMPLEMENT(PopUpElement)