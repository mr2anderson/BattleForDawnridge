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



#include "UpgradeableB.hpp"


#pragma once


class Arable : public UpgradeableB {
public:
	Arable();
	Arable(uint32_t x, uint32_t y, std::shared_ptr<Player> player);
	Building* cloneBuilding() const override;

	Events newMove(std::shared_ptr<Player> player) override;
	Resources getCost() const override;
	std::string getTextureName() const override;
	std::string getSoundName() const override;
	std::wstring getDescription() const override;
	bool alreadyCollected() const;
	uint32_t collect() const;
private:
	bool collected;

	static uint32_t GET_COLLECTION_SPEED(uint32_t i);
	GameActionWindowComponent getUpgradeComponent() override;
	uint32_t getRegenerationSpeed() const override;
	std::wstring getUpperCaseReadableName() const override;
	Resources getUpgradeCost() const override;
	uint32_t getUpgradeTime() const override;
	Events getSelectionW();
	Events getGameObjectResponse(std::shared_ptr<Player> player) override;
};