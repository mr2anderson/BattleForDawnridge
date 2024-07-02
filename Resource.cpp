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


#include "Resource.hpp"


Resource::Resource() = default;
Resource::Resource(const std::string& type, uint32_t n) {
	this->type = type;
	this->n = n;
}
std::wstring Resource::getReadableInfo() const {
	std::wstring result;
	if (this->n == 0) {
		return result;
	}

	if (this->type == "food") {
		result = L"еда (";
	}
	else if (this->type == "wood") {
		result = L"древесина (";
	}
	else if (this->type == "stone") {
		result = L"камень (";
	}
	else if (this->type == "iron") {
		result = L"железо (";
	}
	else if (this->type == "gold") {
		result = L"золото (";
	}
	result = result + std::to_wstring(this->n) + L")";
	return result;
}