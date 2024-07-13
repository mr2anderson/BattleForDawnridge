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


#include "HpSensitiveB.hpp"
#include "AreaBLandscapeSensible.hpp"
#include "ResourcePoint.hpp"
#include "GOCollection.hpp"


#pragma once


class ResourceB : public HpSensitiveB, public AreaBLandscapeSensible {
public:
	ResourceB();
	ResourceB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH);

	Events newMove(uint32_t playerId) override;
protected:
	virtual std::string getResourceType() const = 0;
	virtual uint32_t getCollectionSpeed() const = 0;
private:
	std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr;

    bool ignoreHighObstacles() const override;
    bool ignoreLowObstacles() const override;
	Events tryToCollectResources();
	Events getSelectionW();
	Events getGameObjectResponse(uint32_t playerId) override;
};