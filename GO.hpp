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


#include "HorizontalSelectionWindow.hpp"


#pragma once


class GO : public sf::Drawable {
public:
	GO();
	GO(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	uint32_t getX() const;
	uint32_t getY() const;
	uint32_t getSX() const;
	uint32_t getSY() const;
	void setX(uint32_t newX);
	void setY(uint32_t newY);
	bool blockForWarrior(uint32_t warriorPlayerId) const;
	virtual bool exist() const = 0;
	Events click(uint32_t currentPlayerId, uint32_t mouseX, uint32_t mouseY);
	bool intersects(GO* go) const;

	virtual Events newMove(uint32_t currentPlayerId) = 0;
	virtual std::string getTextureName() const = 0;
	virtual std::string getSoundName() const = 0;
	virtual std::wstring getDescription() const = 0;
protected:
	virtual Events getGameObjectResponse(uint32_t currentPlayerId) = 0;

	virtual HorizontalSelectionWindowComponent getExitComponent() const;
	HorizontalSelectionWindowComponent getDescriptionComponent() const;
private:
	uint32_t x, y;
	uint32_t sx, sy;

	void drawTexture(sf::RenderTarget& target, sf::RenderStates states) const;
};