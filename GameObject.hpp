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


#include "TextureStorage.hpp"
#include "GameObjectResponse.hpp"
#include "HPBar.hpp"
#include "Player.hpp"


#pragma once


class GameObject : public sf::Drawable {
public:
	GameObject(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t currentHp, uint32_t maxHp);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void addHp(uint32_t value);
	void subHp(uint32_t value);
	void changeMaxHp(uint32_t value);
	uint32_t getHP() const;
	uint32_t getMaxHP() const;
	uint32_t getX() const;
	uint32_t getY() const;
	uint32_t getSX() const;
	uint32_t getSY() const;
	bool exist() const;
	GameObjectResponse click(const Player& currentPlayer, uint32_t mouseX, uint32_t mouseY);
	virtual GameObjectResponse newMove(const Player& currentPlayer);
protected:
	virtual std::string getTextureName() const = 0;
	virtual GameObjectResponse getGameObjectResponse(const Player& currentPlayer) = 0;
private:
	HPBar bar;
};