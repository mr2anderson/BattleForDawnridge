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


#include "Logs.hpp"


Logs::Logs() {
	this->limit = 0;
}
void Logs::setEntryLimit(uint32_t newLimit) {
	this->limit = newLimit;
}
uint32_t Logs::getEntryLimit() const {
	return this->limit;
}
void Logs::add(const StringLcl& str) {
	this->content.push_back(str);
	while (this->content.size() > this->limit) {
		this->content.pop_front();
	}

	this->contentCached.clear();
	for (const auto& a : this->content) {
		this->contentCached = this->contentCached + a + "\n\n";
	}
}
StringLcl Logs::get() const {
	return this->contentCached;
}