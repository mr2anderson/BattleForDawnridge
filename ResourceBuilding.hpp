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


#include "UpgradeableHpSensitiveBuilding.hpp"
#include "ResourcePoint.hpp"


#pragma once


class ResourceBuilding : public UpgradeableHpSensitiveBuilding {
public:
	ResourceBuilding();
	ResourceBuilding(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr);

	GameObjectResponse newMove(const Player& currentPlayer) override;
protected:
	virtual std::string getResourceType() const = 0;
	uint32_t getResourceNPerMove() const;
	uint32_t getRadius() const;
	virtual uint32_t getResourceNPerMove(uint32_t level) const = 0;
	virtual uint32_t getRadius(uint32_t level) const = 0;
	virtual std::wstring getDescription() const = 0;
	virtual std::wstring getResourcesOverStr() const = 0;
private:
	const std::vector<ResourcePoint*>* resourcePointsPtr;
	bool resourcesLeft;

	GameObjectResponse collectResources();
	GameObjectResponse highlightArea() const;
	GameObjectResponse getSelectWindow(const GameEvent& highlightEvent);

	GameObjectResponse getGameObjectResponse(const Player& player) override;
	bool inRadius(ResourcePoint* rp) const;
	std::tuple<uint32_t, uint32_t> getCenter() const;
};