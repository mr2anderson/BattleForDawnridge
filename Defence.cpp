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
#include "Texts.hpp"


Defence::Defence(double cut, double stab, double crush) {
	this->cut = cut;
	this->stab = stab;
	this->crush = crush;
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
static std::wstring FORMAT_K(double a) {
	std::wstringstream ss;
	ss << std::setprecision(8) << std::noshowpoint << a;
	return ss.str();
}
std::wstring Defence::getReadable() const {
	std::wstring result;

	result += *Texts::get()->get("cut") + L"[" + FORMAT_K(this->cut) + L"] ";
	result += *Texts::get()->get("stab") + L"[" + FORMAT_K(this->stab) + L"] ";
	result += *Texts::get()->get("crush") + L"[" + FORMAT_K(this->crush) + L"]";

	return result;
}