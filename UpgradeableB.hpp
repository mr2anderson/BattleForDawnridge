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
#include "TryToUpgradeEvent.hpp"
#include "DecreaseUpgradeMovesLeftEvent.hpp"
#include "IncreaseLevelEvent.hpp"
#include "SubResourcesEvent.hpp"


#pragma once


class UpgradeableB : virtual public Building {
public:
	UpgradeableB();
	UpgradeableB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, std::shared_ptr<Player> playerPtr);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Events startUpgrade();
	void decreaseUpgradeMovesLeft();
	void increaseLevel();
	bool works() const override;

	virtual Resources getUpgradeCost() const = 0;
protected:
	static constexpr uint32_t TOTAL_LEVELS = 3;

	virtual uint32_t getUpgradeTime() const = 0;

	uint32_t getCurrentLevel() const;
	bool upgrading() const;
	Events handleCurrentUpgrade();
	Events handleBusyWithUpgrading() const;
private:
	uint32_t currentLevel;
	uint32_t upgradeMovesLeft;

	Events handleUpgradeStart() const;
	Events handleUpgradeEnd();
	void drawCurrentLevel(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawUpgrading(sf::RenderTarget& target, sf::RenderStates states) const;
};