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
#include "HpSensitiveB.hpp"
#include "AreaB.hpp"
#include "ResourcePoint.hpp"
#include "CollectEvent.hpp"
#include "GOCollection.hpp"


#pragma once


class ResourceB : public UpgradeableB, public HpSensitiveB, public AreaB {
public:
	ResourceB();
	ResourceB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, std::shared_ptr<Player> playerPtr, std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr);

	Events newMove(std::shared_ptr<Player> currentPlayer) override;
	bool works() const override;
protected:
	uint32_t getCollectionSpeed() const;
	uint32_t getRadius() const override;

	virtual std::string getResourceType() const = 0;
	virtual uint32_t getCollectionSpeed(uint32_t level) const = 0;
	virtual uint32_t getRadius(uint32_t level) const = 0;
private:
	std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr;
	bool resourcesLeft;

	GameActionWindowComponent getUpgradeComponent() override;
	Events collectResources();
	Events getSelectionW();
	Events getGameObjectResponse(std::shared_ptr<Player> player) override;
};