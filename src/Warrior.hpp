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
#include "IWithSuspendingAnimation.hpp"
#include "MovementGraph.hpp"
#include "AnimationState.hpp"
#include "Damage.hpp"
#include "SpecialMove.hpp"
#include "Clock.hpp"
#include "Timer.hpp"
#include "SpecialAnimation.hpp"
#include "Name.hpp"


#pragma once


class Warrior : public Unit, public IWithSuspendingAnimation {
public:
	Warrior();
	Warrior(uint32_t x, uint32_t y, uint32_t playerId);
    virtual std::shared_ptr<Warrior> cloneWarrior() const = 0;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    StringLcl getNameString() const;
	Events hit(Damage d) override;
    Events heal();
    Events inspire();
	Events killNextTurn();
	Events revertKillNextTurn();
	void enableRageMode();
	void decreaseRageModeMovesLeft();
    bool inRage() const;
	void changeDirection(const std::string& newDirection);
	Events newMove(MapState *state, uint32_t playerId) override;
    Events endMove(MapState *state, uint32_t playerId) override;
	void refreshMovementPoints();
    void wipeMovementPoints();
    uint32_t getSX() const override;
    uint32_t getSY() const override;
	std::string getTextureName() const override;
	uint32_t getAnimationNumber(const std::string& type, const std::string& direction) const;
    uint32_t getCurrentAnimationMs() const;
	bool canStay(MapState *state, uint32_t newX, uint32_t newY) const;
	bool warriorCanStay(std::shared_ptr<const Warrior> w) const override;
	uint32_t getWarriorMovementCost(std::shared_ptr<Warrior> w) const override;
    DrawingPriority getDrawingPriority() const override;
    ClickPriority getClickPriority() const override;
    NewMoveMainPriority getNewMoveMainPriority() const override;
    Events processCurrentAnimation(MapState *state) override;
	void startAnimation(const std::string& type);
    std::string getCurrentAnimation() const;
    AnimationState getCurrentAnimationState() const;
	Defence getDefence() const override;
    std::vector<std::tuple<uint32_t, uint32_t>> getMoves(MapState *state);
    Events getMoveHighlightionEvent(MapState *state);
    StringLcl getDetailedDescription(MapState *state) const override;
    bool wasHealed() const;
    void refreshWasHealedStatus();
    void setPoisonStatus();
    void wipePoisonStatus();
    void setInspiredStatus();
    void wipeInspiredStatus();
    bool isInspired() const;

    virtual bool isFemale() const;
    virtual bool isVehicle() const;
    virtual bool isFlying() const;
    virtual StringLcl getSpecialInfoString(MapState *state) const = 0;
    virtual std::string getBeenHitSoundName() const = 0;
	virtual uint32_t getTimeToProduce() const = 0;
	virtual std::string getBaseTextureName() const = 0;
	virtual uint32_t getTalkingAnimationsNumberInSet() const = 0;
    virtual bool delayBetweenTalkingAnimations() const;
	virtual uint32_t getRunningAnimationsNumberInSet() const = 0;
	virtual uint32_t getBeenHitAnimationsNumberInSet() const = 0;
	virtual uint32_t getTippingOverAnimationsNumberInSet() const = 0;
	virtual uint32_t getPopulation() const = 0;
	virtual uint32_t getMovementPoints() const = 0;
    virtual bool blockBuildingAbility() const = 0;
    virtual bool isInspirable() const;

	static const uint32_t TOTAL_FOOTSTEPS;
    static const uint32_t TOTAL_WINGS;
protected:
    virtual boost::optional<SpecialAnimation> getSpecialAnimation() const = 0;
    virtual Events processSpecialAnimation() = 0;
    virtual std::vector<SpecialMove> getSpecialMoves(MapState *state) const = 0;
    virtual Events handleSpecialMove(MapState *state, uint32_t targetX, uint32_t targetY) = 0;
	virtual Defence getBaseDefence() const = 0;
private:
    Name name;
	boost::optional<uint32_t> movementPoints;
    bool hasSpecialMoves;
    bool enemyMove;
    bool wasHealedThisMove;
	std::string defaultDirection;
	std::string currentAnimation;
	Clock animationClock;
    std::vector<std::string> currentMovement;
    Clock footstepsClock;
	bool toKill;
	uint32_t rageModeMovesLeft;
    bool poison;
    bool inspired;

	std::shared_ptr<sf::Drawable> getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const override;
    void update(MapState *state, uint32_t playerId) override;
	Events onUnselect(MapState *state, uint32_t x, uint32_t y, uint8_t button) override;
    Move getMove(MapState *state, uint32_t x2, uint32_t y2);
    MovementGraph buildMovementGraph(MapState *state);
    Events processRunningAnimation(MapState *state);
	Events processBeenHitAnimation();
	Events processTippingOverAnimation();
    float getOffsetX() const override;
    float getOffsetY() const override;
    float getOffset(const std::string &toNeg, const std::string &toPos) const;
	sf::Color getTextureColor() const override;
	HorizontalSelectionWindowComponent getRageModeComponent() const;
    HorizontalSelectionWindowComponent getPoisonComponent() const;
    HorizontalSelectionWindowComponent getInspiredComponent() const;
    HorizontalSelectionWindowComponent getTimeModComponent(MapState *state) const;
	HorizontalSelectionWindowComponent getKillComponent();
	HorizontalSelectionWindowComponent getRevertKillComponent();
    HorizontalSelectionWindowComponent getWarriorInfoComponent(MapState *state) const;
    HorizontalSelectionWindowComponent getWarriorOfEnemyComponent() const;
    Events getSelectionWindow(MapState *state, bool own, bool minimal);
    void addHp(uint32_t value) override;

	Events getResponse(MapState *state, uint32_t playerId, uint32_t button) override;
    std::shared_ptr<PlayerPointer> getPlayerPointer() const override;
	void drawHPPointer(sf::RenderTarget& target, sf::RenderStates states) const;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Unit>(*this);
        ar & this->name;
        ar & this->movementPoints;
        ar & this->hasSpecialMoves;
        ar & this->enemyMove;
        ar & this->wasHealedThisMove;
        ar & this->defaultDirection;
        ar & this->currentAnimation;
        ar & this->animationClock;
        ar & this->currentMovement;
        ar & this->toKill;
        ar & this->rageModeMovesLeft;
        ar & this->poison;
        ar & this->inspired;
    }
};


BOOST_CLASS_EXPORT_KEY(Warrior)