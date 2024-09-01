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


#include <iomanip>
#include <sstream>
#include "Defence.hpp"
#include "Locales.hpp"


Defence::Defence() = default;
Defence::Defence(double cut, double stab, double crush) {
	this->cut = cut;
	this->stab = stab;
	this->crush = crush;
}
Defence operator*(double k, Defence defence) {
	return Defence(defence.cut * (2 - k), defence.stab * (2 - k), defence.crush * (2 - k));
}
double Defence::getCut() const {
	return this->cut;
}
double Defence::getStab() const {
	return this->stab;
}
double Defence::getCrush() const {
	return this->crush;
}
StringLcl Defence::getReadable() const {
	StringLcl result;

	result = result + StringLcl("{cut} " + std::to_string((uint32_t)(this->cut * 100)) + "% ");
	result = result + StringLcl("{stab} " + std::to_string((uint32_t)(this->stab * 100)) + "% ");
	result = result + StringLcl("{crush} " + std::to_string((uint32_t)(this->crush * 100)) + "%");

	return result;
}