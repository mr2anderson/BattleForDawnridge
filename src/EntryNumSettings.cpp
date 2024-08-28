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


#include <limits>
#include "EntryNumSettings.hpp"


EntryNumSettings::EntryNumSettings() {
	this->max = std::numeric_limits<uint64_t>::max() / 100;
}
void EntryNumSettings::setMax(uint64_t newMax) {
	this->max = newMax;
}
bool EntryNumSettings::ok(std::shared_ptr<std::string> dstPtr) const {
	return !dstPtr->empty() and std::stoull(*dstPtr) <= this->max;
}
static uint64_t VAL(std::shared_ptr<std::string> ptr) {
	if (ptr->empty()) {
		return 0;
	}
	return std::stoull(*ptr);
}
void EntryNumSettings::mod(std::shared_ptr<std::string> dstPtr, sf::Keyboard::Key key) {
	if (key == sf::Keyboard::BackSpace) {
		if (!dstPtr->empty()) {
			dstPtr->pop_back();
		}
	}
	else if (key >= sf::Keyboard::Key::Num0 and key <= sf::Keyboard::Key::Num9 and (dstPtr->empty() or dstPtr->front() != '0')) {
		if (VAL(dstPtr) * 10 + (key - sf::Keyboard::Key::Num0) <= this->max) {
			dstPtr->push_back('0' + (key - sf::Keyboard::Key::Num0));
		}
	}
}