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


#include "Unit.hpp"
#include "MessageWindow.hpp"


#pragma once


class Building : public Unit {
public:
	Building(uint32_t x, uint32_t y, uint32_t maxHp, bool full, const Player *playerPtr);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual GameObjectResponse upgrade() = 0;
	virtual Resources getCost() const = 0;
protected:
	virtual uint32_t getRegenerationSpeed() const = 0;
	virtual std::string getNewWindowSoundName() const = 0;
	virtual std::wstring getUpgradeStartDescription() const = 0;
	virtual std::wstring getUpgradeFinishDescription() const = 0;
	virtual std::wstring getBusyWithUpgradingDescription() const = 0;

	virtual GameObjectResponse processRegeneration();
	virtual GameObjectResponse decreaseUpgradeMovesLeft();
	uint32_t getCurrentLevel() const;
	bool upgrading() const;
	GameObjectResponse handleUpgradeStart() const;
	GameObjectResponse handleUpgrading() const;
	void upgrade(uint32_t t);
private:
	uint32_t currentLevel;
	uint32_t upgradeMovesLeft;

	void drawCurrentLevel(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawUpgrading(sf::RenderTarget& target, sf::RenderStates states) const;
};