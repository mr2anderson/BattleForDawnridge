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


#include "PopUpElement.hpp"
#include "TerritoryB.hpp"


#pragma once


class BuildingMode : public PopUpElement {
public:
	BuildingMode(Building *b, sf::View *view, std::vector<GO*> *go, std::vector<TerritoryB*> *tb, Player *player);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Event run(uint32_t windowW, uint32_t windowH) override;
	void update() override;
	Event click() override;
private:
	Building* b;
	sf::View* view;
	uint32_t windowW, windowH;
	std::vector<GO*> *go;
	std::vector<TerritoryB*> *tb;
	Player* player;

	bool empty() const;
	bool controlled() const;
	Event getHighlightEvent() const;
};