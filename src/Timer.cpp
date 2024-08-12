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


#include "Timer.hpp"


Timer::Timer() {
	this->ms = 0;
	this->type = TYPE::FIRST_DEFAULT;
	this->first = false;
}
Timer::Timer(uint32_t ms, uint8_t type) {
	this->ms = ms;
	this->type = type;
	this->first = false;
}
bool Timer::ready() const {
	return ((this->first and type == TYPE::FIRST_INSTANTLY) or this->clock.getMS() > this->ms);
}
void Timer::reset() {
	this->clock.restart();
	this->first = false;
}