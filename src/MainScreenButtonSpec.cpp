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


#include "MainScreenButtonSpec.hpp"


MainScreenButtonSpec::MainScreenButtonSpec() {
	this->index = 0;
}
MainScreenButtonSpec::MainScreenButtonSpec(uint32_t index) {
	this->index = index;
}
uint32_t MainScreenButtonSpec::getX() const {
	return 5;
}
uint32_t MainScreenButtonSpec::getY() const {
	return 40 + this->index * (64 + 10);
}
uint32_t MainScreenButtonSpec::getW() const {
	return 200;
}
uint32_t MainScreenButtonSpec::getH() const {
	return 64;
}