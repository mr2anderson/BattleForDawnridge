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


#include "CameraDependentPopUpElement.hpp"
#include "TerritoryB.hpp"
#include "GOCollection.hpp"


#pragma once


class BuildingMode : public CameraDependentPopUpElement {
public:
	BuildingMode(std::shared_ptr<Building> b, std::shared_ptr<sf::View> view, std::shared_ptr<GOCollection<GO>> go, std::shared_ptr<GOCollection<TerritoryB>> tb, uint32_t playerId, uint32_t mapW, uint32_t mapH);
	~BuildingMode();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Events run(uint32_t windowW2, uint32_t windowH2) override;
	void update() override;
	Events click() override;
private:
	Building* b;
	std::shared_ptr<sf::View> view;
	uint32_t windowW, windowH;
	std::shared_ptr<GOCollection<GO>> go;
	std::shared_ptr<GOCollection<TerritoryB>> tb;
	uint32_t playerId;
	uint32_t mapW, mapH;
	bool returnedPtr;

	bool inMap() const;
	bool empty() const;
	bool controlled() const;
	Events getHighlightEvent() const;
};