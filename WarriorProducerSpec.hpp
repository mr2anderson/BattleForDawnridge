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


#include "AreaControllerSpec.hpp"


#pragma once


class WarriorProducerSpec : public AreaControllerSpec {
public:
	WarriorProducerSpec();

	Events startProducing(std::shared_ptr<Warrior> w);
	void decreaseCurrentProducingMovesLeft();
	void stopProducing();
	Events getActiveNewMoveEvent(const Building *b, MapState* state) override;
	std::vector<HorizontalSelectionWindowComponent> getComponents(const Building *b, MapState* state) override;
	std::optional<BuildingShortInfo> getShortInfo(const Building *b) const override;
	uint32_t getRadius() const override;
	virtual std::vector<std::shared_ptr<Warrior>> getWarriorsToProduce(uint32_t playerId) = 0;
private:
	std::shared_ptr<Warrior> currentProducing;
	uint32_t currentProducingMovesLeft;
	bool producing;

	std::tuple<uint32_t, uint32_t> getNewWarriorPosition(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, MapState* state);
};