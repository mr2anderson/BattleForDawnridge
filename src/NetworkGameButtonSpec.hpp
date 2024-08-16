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


#include <vector>
#include <set>
#include "MenuButtonSpec.hpp"


#pragma once


class NetworkGameButtonSpec : public MenuButtonSpec {
public:
	NetworkGameButtonSpec();
	NetworkGameButtonSpec(uint32_t index);
	NetworkGameButtonSpec(uint32_t index, const std::vector<std::string>& mapNames, const std::set<std::string>& saveNames);
private:
	std::vector<std::string> mapNames;
	std::set<std::string> saveNames;

	StringLcl getString() const override;
	Events getEvents() const override;

	Events getChooseMapEvent() const;
	Events getChooseSaveEvent() const;

	Events getChooseMapEvent(const std::string& mapName) const;
};