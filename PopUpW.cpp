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


#include "PopUpW.hpp"


PopUpW::PopUpW(const std::string& soundName1, const std::string& soundName2) {
	this->soundName1 = soundName1;
	this->soundName2 = soundName2;
}
void PopUpW::run(uint32_t windowW, uint32_t windowH) {
	this->playSound1();
}
void PopUpW::playSound1() const {
	if (this->soundName1 != "") {
		SoundQueue::get()->push(Sounds::get()->get(this->soundName1));
	}
}
void PopUpW::playSound2() const {
	if (this->soundName2 != "") {
		SoundQueue::get()->push(Sounds::get()->get(this->soundName2));
	}
}