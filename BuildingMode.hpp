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


#include "PopUpElement.hpp"
#include "TerritoryB.hpp"


#pragma once


class BuildingMode : public PopUpElement {
public:
	BuildingMode(Building *b, uint32_t *mouseX, uint32_t *mouseY, sf::View *view, std::vector<GO*> *go, std::vector<TerritoryB*> *tb, Player *player);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Event run(uint32_t windowW, uint32_t windowH) override;
	void update() override;
	Event click(uint32_t x, uint32_t y) override;
private:
	Building* b;
	uint32_t* mouseX, *mouseY;
	sf::View* view;
	uint32_t windowW, windowH;
	std::vector<GO*> *go;
	std::vector<TerritoryB*> *tb;
	Player* player;

	bool empty() const;
	bool controlled() const;
	Event getHighlightEvent() const;
};