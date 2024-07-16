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


#include "Building.hpp"
#include "Dragon.hpp"


#pragma once


class DragonCave : virtual public Building {
public:
	DragonCave();
	DragonCave(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units);
	~DragonCave();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Events setDragon(Dragon *newDragon);
	void decreaseDragonRecoverMovesLeft();
protected:
	Events processDecreaseDragonRecoverMovesLeft();
	std::vector<HorizontalSelectionWindowComponent> getDragonCaveComponents();
	virtual HorizontalSelectionWindowComponent getRaiseComponent(std::shared_ptr<Dragon> newDragon);
	virtual HorizontalSelectionWindowComponent getAttackComponent();
private:
	Dragon* dragon;

	HorizontalSelectionWindowComponent getRestComponent();
	void drawCurrentDragonBuildingShortInfo(sf::RenderTarget& target, sf::RenderStates states) const;
};