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


#include "ResourcePoint.hpp"


#pragma once


class Treasure : public ResourcePoint {
public:
	Treasure();
	Treasure(uint32_t x, uint32_t y);

	Events newMove(uint32_t playerId) override;
	Events tryToCollect(uint32_t playerId, uint32_t value) override;

	std::string getResourceType() const override;
	std::string getSoundName() const override;
private:
	bool alreadyCollected;

	std::string getTextureName() const override;
	std::wstring getDescription() const override;
};