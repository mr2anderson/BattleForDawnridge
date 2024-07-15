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


#include "HPGO.hpp"


#pragma once


class ResourcePoint : public HPGO {
public:
	ResourcePoint();
	ResourcePoint(uint32_t x, uint32_t y);

	Events newMove(uint32_t playerId) override;
	virtual Events tryToCollect(uint32_t playerId, uint32_t value);
	virtual std::string getResourceType() const = 0;
	bool warriorCanStay(uint32_t warriorPlayerId) const override;
	uint32_t getWarriorMovementCost(uint32_t warriorPlayerId) const override;
private:
	std::shared_ptr<HPPointer> getHPPointer() const override;
	Events getGameObjectResponse(uint32_t playerId) override;
	Events getSelectionWindow();
	HorizontalSelectionWindowComponent getResourceLeftComponent() const;
};