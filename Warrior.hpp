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


#include <optional>
#include "Unit.hpp"
#include "GOCollection.hpp"
#include "Selectable.hpp"
#include "Animated.hpp"
#include "MovementGraph.hpp"
#include "AnimationState.hpp"


#pragma once


class Warrior : public Unit, public Selectable, public Animated {
public:
	Warrior();
	Warrior(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH);
	virtual Warrior* cloneWarrior() const = 0;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	Events newMove(uint32_t playerId) override;
	void refreshMovementPoints();
	void startClickAnimation();
    uint32_t getSX() const override;
    uint32_t getSY() const override;
	std::string getTextureName() const override;
	uint32_t getAnimationNumber(const std::string& type, const std::string& direction) const;
    uint32_t getCurrentAnimationMs() const;
	bool canStay(uint32_t newX, uint32_t newY) const;
	uint32_t getMovementCost(uint32_t newX, uint32_t newY) const;
	bool warriorCanStay(uint32_t warriorPlayerId) const override;
	uint32_t getWarriorMovementCost(uint32_t warriorPlayerId) const override;
    Events processCurrentAnimation() override;

	virtual uint32_t getTimeToProduce() const = 0;
	virtual std::string getBaseTextureName() const = 0;
	virtual uint32_t getTalkingAnimationsNumberInSet() const = 0;
	virtual uint32_t getWalkingAnimationsNumberInSet() const = 0;
	virtual uint32_t getAttackAnimationsNumberInSet() const = 0;
	virtual uint32_t getBeenHitAnimationsNumberInSet() const = 0;
	virtual uint32_t getTippingOverAnimationsNumberInSet() const = 0;
protected:
	virtual uint32_t getMovementPoints() const = 0;
private:
	std::optional<uint32_t> movementPoints;
	std::shared_ptr<GOCollection<GO>> go;
	uint32_t mapW, mapH;
	std::string currentDirection;
	std::string currentAnimation;
	sf::Clock animationClock;
    std::queue<std::string> currentMovement;

	bool highDrawingPriority() const override;
	bool highClickPriority() const override;
	std::string getSelectableTextureName() const override;
	Events unselect(uint32_t x, uint32_t y) override;
	Events unselect() override;
	Events getMoveHighlightionEvent();
	std::vector<std::tuple<uint32_t, uint32_t>> getMoves();
    Move getMove(uint32_t x2, uint32_t y2);
    MovementGraph buildMovementGraph();
    Events processWalkingAnimation();
    float getOffsetX() const override;
    float getOffsetY() const override;
    float getOffset() const;
	void startAnimation(const std::string &type);
    AnimationState getCurrentAnimationState() const;
	Events getGameObjectResponse(uint32_t playerId) override;
    std::shared_ptr<PlayerPointer> getPlayerPointer() const override;
};