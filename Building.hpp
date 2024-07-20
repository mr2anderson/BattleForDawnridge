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


#include "Unit.hpp"
#include "Fire.hpp"
#include "IBuildingSpec.hpp"


#pragma once


class Building : public Unit {
public:
	Building();
	Building(uint32_t x, uint32_t y, uint32_t playerId);
	Building(const Building& building);
	~Building();
	virtual Building* cloneBuilding() const = 0;

	Events getHighlightEvent(MapState *state) const;
	Resources getLimit() const;
	uint32_t getPopulationLimit() const;
	bool isVictoryCondition() const;
	bool isOrigin() const;
	bool isActiveConductor() const;
	bool works() const;
	bool connectedTo(MapState* state, GO* go) const;
	bool connectedToOrigin(MapState* state) const;
	bool allowBuilding(MapState* state, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy);
	Events destroy(MapState *state);
	void decreaseBurningMovesLeft();
	void setFire();
protected:
	void addSpec(IBuildingSpec* spec);
	virtual uint32_t getRegenerationSpeed() const = 0;
	virtual std::wstring getUpperCaseReadableName() const = 0;
private:
	uint32_t burningMovesLeft;
	Fire fire;
	std::vector<IBuildingSpec*> specs;

	bool isUltraHighObstacle(uint32_t playerId) const override;
	bool isHighObstacle(uint32_t playerId) const override;
	bool isLowObstacle(uint32_t playerId) const override;
	HorizontalSelectionWindowComponent getHpInfoComponent() const;
	HorizontalSelectionWindowComponent getDestroyComponent();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void drawShortInfos(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawHPPointer(sf::RenderTarget& target, sf::RenderStates states) const;
	Events hit(uint32_t d, const std::optional<std::string>& direction) override;
	Events newMove(MapState* state, uint32_t playerId) override;
	Events processRegeneration();
	Events getResponse(MapState *state, uint32_t playerId, uint32_t button);
	sf::Color getTextureColor() const override;
	bool warriorCanStay(uint32_t warriorPlayerId) const override;
	uint32_t getWarriorMovementCost(uint32_t warriorPlayerId) const override;
    std::shared_ptr<PlayerPointer> getPlayerPointer() const override;
};