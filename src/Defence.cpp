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


#include "Defence.hpp"
#include "Locales.hpp"
#include "ColorTheme.hpp"


Defence::Defence() = default;
Defence::Defence(double cut, double stab, double crush, double magick) {
	this->cut = cut;
	this->stab = stab;
	this->crush = crush;
    this->magick = magick;
}
Defence operator*(double k, Defence defence) {
	return Defence(defence.cut * (2 - k), defence.stab * (2 - k), defence.crush * (2 - k), defence.magick * (2 - k));
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
double Defence::getMagick() const {
    return this->magick;
}
StringLcl Defence::getReadable() const {
	StringLcl result;

    auto cutInt = (uint32_t)(this->cut * 100);
    auto stabInt = (uint32_t)(this->stab * 100);
    auto crushInt = (uint32_t)(this->crush * 100);
    auto magickInt = (uint32_t)(this->magick * 100);

	result = result + StringLcl("{cut} ") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_REV<uint32_t>(cutInt, 10, 100)) + std::to_string(cutInt) + "% ";
	result = result + StringLcl("{stab} ") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_REV<uint32_t>(stabInt, 10, 100)) + std::to_string(stabInt) + "% ";
	result = result + StringLcl("{crush} ") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_REV<uint32_t>(crushInt, 10, 100)) + std::to_string(crushInt) + "% ";
    result = result + StringLcl("{magick} ") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_REV<uint32_t>(magickInt, 10, 100)) + std::to_string(magickInt) + "%";

	return result;
}