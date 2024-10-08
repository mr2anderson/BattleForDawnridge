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


#include "Resource.hpp"
#include "Locales.hpp"
#include "ColorTheme.hpp"


Resource::Resource() = default;
Resource::Resource(const std::string& type, uint32_t n) {
	this->type = type;
	this->n = n;
}
StringLcl Resource::getReadableInfo() const {
	StringLcl result;
	if (this->n == 0) {
		return result;
	}

	result = result + StringLcl("{" + this->type + "_number} ");
	result = result + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + StringLcl(std::to_string(this->n));

	return result;
}