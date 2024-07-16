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


#include "TerritoryB.hpp"
#include "GOCollection.hpp"
#include "Selectable.hpp"


#pragma once


class BuildingMode : public Selectable {
public:
    BuildingMode();
	BuildingMode(std::shared_ptr<const Building> b, std::shared_ptr<GOCollection<GO>> go, std::shared_ptr<GOCollection<TerritoryB>> tb, uint32_t playerId, uint32_t mapW, uint32_t mapH);

	Events start();
private:
	std::shared_ptr<const Building> b;
	uint32_t windowW, windowH;
	std::shared_ptr<GOCollection<GO>> go;
	std::shared_ptr<GOCollection<TerritoryB>> tb;
	uint32_t playerId;
	uint32_t mapW, mapH;

	std::shared_ptr<sf::Drawable> getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const override;
	Events unselect(uint32_t x, uint32_t y, uint8_t button) override;

	Events getHighlightEvent() const;

	bool inMap(Building *clonedB) const;
	bool empty(Building *clonedB) const;
	bool controlled(Building *clonedB) const;
};