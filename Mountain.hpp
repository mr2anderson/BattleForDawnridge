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


#include <random>
#include "ResourcePoint.hpp"


#pragma once


class Mountain : public ResourcePoint {
public:
	Mountain(uint32_t x, uint32_t y);
	std::string getResourceType() const override;

	static constexpr uint32_t TOTAL_TYPES = 7;
private:
	uint32_t type;

	std::string getTextureName() const override;
	std::string getIconName() const override;
	std::wstring getDescription() const override;
	std::string getClickSoundName() const override;
};