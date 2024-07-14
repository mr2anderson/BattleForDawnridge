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


#include "Unit.hpp"
#include "WindowButton.hpp"
#include "CreateEEvent.hpp"
#include "AddHpEvent.hpp"
#include "WindowTwoButtons.hpp"
#include "DestroyEvent.hpp"


#pragma once


class Building : public Unit {
public:
	Building();
	Building(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units);
	virtual Building* cloneBuilding() const = 0;

	virtual bool works() const;
	virtual Events destroy();
protected:
	virtual uint32_t getRegenerationSpeed() const = 0;
	virtual std::wstring getUpperCaseReadableName() const = 0;

	virtual Events regenerate();
	HorizontalSelectionWindowComponent getHpInfoComponent() const;
	virtual HorizontalSelectionWindowComponent getDestroyComponent();
private:
	bool warriorCanStay(uint32_t warriorPlayerId) const override;
	uint32_t getWarriorMovementCost(uint32_t warriorPlayerId) const override;
    virtual std::shared_ptr<PlayerPointer> getPlayerPointer() const override;
};