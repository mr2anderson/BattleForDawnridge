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
#include "GlobalRandomGenerator64.hpp"
#include "math.hpp"
#include "InvalidRoomIDFormat.hpp"


static constexpr uint32_t CHAR_NUMBER = 26 + 10;
static constexpr uint32_t READABLE_LEN = 12;
static constexpr uint64_t COMBINATIONS = bfdlib::math::pow<uint64_t>(CHAR_NUMBER, READABLE_LEN);
static constexpr std::array<uint64_t, READABLE_LEN> EXPONENTS = {
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 0),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 1),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 2),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 3),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 4),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 5),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 6),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 7),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 8),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 9),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 10),
	 bfdlib::math::pow<uint64_t>(CHAR_NUMBER, 11)
};


static char indexToChar(uint32_t index) {
	if (index < 10) {
		return '0' + index;
	}
	return 'A' + (index - 10);
}
static uint32_t charToIndex(char c) {
	if (c >= '0' and c <= '9') {
		return c - '0';
	}
	return c - 'A' - 10;
}


RoomID::RoomID() {
	this->_value = GlobalRandomGenerator64::get()->gen() % COMBINATIONS;
}
RoomID::RoomID(uint64_t value) {
	this->_value = value;
}
RoomID::RoomID(const std::string& readableValue) {
	if (readableValue.size() != READABLE_LEN) {
		throw InvalidRoomIDFormat();
	}

	this->_value = 0;

	for (uint32_t i = 0; i < READABLE_LEN; i = i + 1) {
		uint64_t m = EXPONENTS.at(READABLE_LEN - i - 1);
		this->_value = this->_value + m * charToIndex(readableValue.at(i));
	}
}


bool RoomID::operator==(const RoomID& b) const {
	return (this->_value == b._value);
}
bool RoomID::operator!=(const RoomID& b) const {
	return (this->_value != b._value);
}


uint64_t RoomID::value() const {
	return this->_value;
}
std::string RoomID::readableValue() const {
	uint64_t n = this->_value;

	std::string result;
	result.resize(READABLE_LEN);

	for (uint32_t i = 0; i < READABLE_LEN; i = i + 1) {
		uint64_t div = EXPONENTS.at(READABLE_LEN - i - 1);
		uint32_t divResult = n / div;
		result.at(i) = indexToChar(divResult);
		n = n % div;
	}

	return result;
}