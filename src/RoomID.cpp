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


#include <array>
#include "RoomID.hpp"
#include "GlobalRandomGenerator32.hpp"
#include "InvalidRoomIDFormat.hpp"


const uint32_t RoomID::READABLE_LEN = 12;


RoomID::RoomID() {
	for (uint32_t i = 0; i < READABLE_LEN; i = i + 1) {
		uint8_t r = GlobalRandomGenerator32::get().gen() % 36;
		if (r < 10) {
			this->_value.push_back('0' + r);
		}
		else {
			this->_value.push_back('A' + (r - 10));
		}
	}
}
RoomID::RoomID(const std::string& readableValue) {
	if (readableValue.size() != READABLE_LEN) {
		throw InvalidRoomIDFormat();
	}

	for (uint32_t i = 0; i < READABLE_LEN; i = i + 1) {
		if (readableValue.at(i) >= '0' and readableValue.at(i) <= '9') {
			continue;
		}
		if (readableValue.at(i) >= 'A' and readableValue.at(i) <= 'Z') {
			continue;
		}
		throw InvalidRoomIDFormat();
	}

    this->_value = readableValue;
}


bool RoomID::operator==(const RoomID& b) const {
	return (this->_value == b._value);
}
bool RoomID::operator!=(const RoomID& b) const {
	return (this->_value != b._value);
}


std::string RoomID::value() const {
	return this->_value;
}