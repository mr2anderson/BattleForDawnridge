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
#include "Collection.hpp"
#include "ISelectable.hpp"
#include "IWithSuspendingAnimation.hpp"
#include "MovementGraph.hpp"
#include "AnimationState.hpp"
#include "Damage.hpp"


#pragma once


class Warrior : public Unit, public ISelectable, public IWithSuspendingAnimation {
public:
	Warrior();
	Warrior(uint32_t x, uint32_t y, uint32_t playerId);
	virtual Warrior* cloneWarrior() const = 0;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	Events hit(uint32_t d) override;
	Events killNextTurn();
	Events revertKillNextTurn();
	void enableRageMode();
	void decreaseRageModeMovesLeft();
	void changeDirection(const std::string& newDirection);
	Events newMove(MapState *state, uint32_t playerId) override;
	void refreshMovementPoints();
    uint32_t getSX() const override;
    uint32_t getSY() const override;
	std::string getTextureName() const override;
	uint32_t getAnimationNumber(const std::string& type, const std::string& direction) const;
    uint32_t getCurrentAnimationMs() const;
	bool canStay(MapState *state, uint32_t newX, uint32_t newY) const;
	bool warriorCanStay(uint32_t warriorPlayerId) const override;
	uint32_t getWarriorMovementCost(uint32_t warriorPlayerId) const override;
    Events processCurrentAnimation() override;
	void startAnimation(const std::string& type);
	Defence getDefence() const override;

    virtual uint8_t getType() const = 0;
	virtual uint32_t getTimeToProduce() const = 0;
	virtual std::string getBaseTextureName() const = 0;
	virtual uint32_t getTalkingAnimationsNumberInSet() const = 0;
    virtual bool delayBetweenTalkingAnimations() const;
	virtual uint32_t getRunningAnimationsNumberInSet() const = 0;
	virtual uint32_t getBeenHitAnimationsNumberInSet() const = 0;
	virtual uint32_t getTippingOverAnimationsNumberInSet() const = 0;
	virtual uint32_t getPopulation() const = 0;
	virtual uint32_t getMovementPoints() const = 0;

	static const uint32_t TOTAL_FOOTSTEPS;

    enum TYPE {
        HUMAN
    };
protected:
	virtual Defence getBaseDefence() const = 0;
private:
	std::optional<uint32_t> movementPoints;
	std::string currentDirection;
	std::string currentAnimation;
	sf::Clock animationClock;
    std::queue<std::string> currentMovement;
	sf::Clock footstepsClock;
	bool toKill;
	uint32_t rageModeMovesLeft;

	uint8_t getDrawingPriority() const override;
	uint8_t getClickPriority() const override;
	std::shared_ptr<sf::Drawable> getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const override;
	Events unselect(MapState *state, uint32_t x, uint32_t y, uint8_t button) override;
	Events getMoveHighlightionEvent(MapState *state);
	std::vector<std::tuple<uint32_t, uint32_t>> getMoves(MapState *state);
    Move getMove(MapState *state, uint32_t x2, uint32_t y2);
    MovementGraph buildMovementGraph(MapState *state);
    Events processRunningAnimation();
	Events processBeenHitAnimation();
	Events processTippingOverAnimation();
    float getOffsetX() const override;
    float getOffsetY() const override;
    float getOffset() const;
	sf::Color getTextureColor() const override;
	float getScale() const override;
	HorizontalSelectionWindowComponent getRageModeComponent() const;
	HorizontalSelectionWindowComponent getKillComponent();
	HorizontalSelectionWindowComponent getRevertKillComponent();
    HorizontalSelectionWindowComponent getWarriorInfoComponent() const;
    HorizontalSelectionWindowComponent getWarriorOfEnemyComponent() const;
    Events getSelectionWindow(bool own);

    AnimationState getCurrentAnimationState() const;
	Events getResponse(MapState *state, uint32_t playerId, uint32_t button) override;
    std::shared_ptr<PlayerPointer> getPlayerPointer() const override;
	void drawHPPointer(sf::RenderTarget& target, sf::RenderStates states) const;
};