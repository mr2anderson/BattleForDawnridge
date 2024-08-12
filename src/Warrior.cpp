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


#include <limits>
#include "Warrior.hpp"
#include "SelectEvent.hpp"
#include "SetHighlightEvent.hpp"
#include "RefreshMovementPointsEvent.hpp"
#include "HighlightColors.hpp"
#include "DisableCursorEvent.hpp"
#include "MoveDoesNotExist.hpp"
#include "CreateAnimationEvent.hpp"
#include "CloseAnimationEvent.hpp"
#include "WindowButton.hpp"
#include "Locales.hpp"
#include "CreateEEvent.hpp"
#include "WarriorPlayerPointer.hpp"
#include "HPFlyingE.hpp"
#include "SubHpEvent.hpp"
#include "Textures.hpp"
#include "FirstTimeTipsTable.hpp"
#include "StartWarriorAnimationEvent.hpp"
#include "WarriorHPPointer.hpp"
#include "UnselectEvent.hpp"
#include "ResetHighlightEvent.hpp"
#include "EnableCursorEvent.hpp"
#include "KillNextTurnEvent.hpp"
#include "RevertKillNextTurnEvent.hpp"
#include "WindowTwoButtons.hpp"
#include "DecreaseRageModeMovesLeftEvent.hpp"
#include "Parameters.hpp"
#include "GlobalRandomGenerator32.hpp"
#include "FocusOnEvent.hpp"
#include "Building.hpp"
#include "IAreaControllerSpec.hpp"
#include "CircleLightSourceSqrt.hpp"
#include "PlaySoundEvent.hpp"


const uint32_t Warrior::TOTAL_FOOTSTEPS = 10;
const uint32_t Warrior::TOTAL_WINGS = 3;


Warrior::Warrior() {
    this->currentDirection = "e";
    this->startAnimation("talking");
}
Warrior::Warrior(uint32_t x, uint32_t y, uint32_t playerId) :
	Unit(x, y, boost::none, playerId) {
	this->movementPoints = boost::none;
    this->currentDirection = "e";
    this->startAnimation("talking");
    this->toKill = false;
    this->rageModeMovesLeft = 0;
    this->hasSpecialMoves = false;
    this->enemyMove = true;
    this->newFrameUpdateTimer = Timer(250, Timer::FIRST_INSTANTLY);
}
void Warrior::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    this->Unit::draw(target, states);
    if (this->exist()) {
        this->drawHPPointer(target, states);
    }
}
Events Warrior::hit(uint32_t d) {
    uint32_t hpPointsAfterOperation;
    if (d >= this->getHP()) {
        hpPointsAfterOperation = 0;
    }
    else {
        hpPointsAfterOperation = this->getHP() - d;
    }

    Events response;

    response.add(std::make_shared<PlaySoundEvent>(this->getBeenHitSoundName()));

    if (hpPointsAfterOperation == 0) {
        response.add(std::make_shared<StartWarriorAnimationEvent>(this, "tipping over"));
    }
    else {
        response.add(std::make_shared<StartWarriorAnimationEvent>(this, "been hit"));
    }

    std::shared_ptr<HPFlyingE> hpFlyingE = std::make_shared<HPFlyingE>(d, false, this->getX(), this->getY(), this->getSX(), this->getSY());
    response.add(std::make_shared<CreateEEvent>(hpFlyingE));

    response.add(std::make_shared<SubHpEvent>(this, this->getHP() - hpPointsAfterOperation - 1 * (hpPointsAfterOperation == 0)));

    response.add(std::make_shared<CreateAnimationEvent>(SuspendingAnimation(this)));

    return response;
}
Events Warrior::killNextTurn() {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{will_be_killed_on_next_turn}"), StringLcl("{OK}"), clickSoundEvent);

    Events event = clickSoundEvent;
    event.add(std::make_shared<CreateEEvent>(w));

    this->toKill = true;

    return event;
}
Events Warrior::revertKillNextTurn() {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{wont_be_killed_on_next_turn}"), StringLcl("{OK}"), clickSoundEvent);

    Events event = clickSoundEvent;
    event.add(std::make_shared<ResetHighlightEvent>());
    event.add(std::make_shared<CreateEEvent>(w));

    this->toKill = false;

    return event;
}
void Warrior::enableRageMode() {
    this->rageModeMovesLeft = Parameters::get()->getInt("rage_mode_lifetime");
}
void Warrior::decreaseRageModeMovesLeft() {
    this->rageModeMovesLeft = this->rageModeMovesLeft - 1;
}
bool Warrior::inRage() const {
    return (this->rageModeMovesLeft > 0);
}
void Warrior::changeDirection(const std::string& newDirection) {
    this->currentDirection = newDirection;
}
Events Warrior::newMove(MapState *state, uint32_t currentPlayerId) {
	if (this->exist()) {
        Events events;

		events.add(std::make_shared<RefreshMovementPointsEvent>(this));

        if (this->belongTo(currentPlayerId)) {
            if (this->inRage()) {
                events.add(std::make_shared<DecreaseRageModeMovesLeftEvent>(this));
            }

            if (this->toKill) {
                events.add(std::make_shared<FocusOnEvent>(this->getX(), this->getY(), this->getSX(), this->getSY()));
                events = events + this->hit(this->getHP());
            }
        }

		return events;
	}
	return Events();
}
void Warrior::refreshMovementPoints() {
	this->movementPoints = this->getMovementPoints();
}
void Warrior::wipeMovementPoints() {
    this->movementPoints = 0;
}
uint32_t Warrior::getSX() const { // Config file is not used cuz engine supports only (1, 1) warriors
    return 1;
}
uint32_t Warrior::getSY() const {
    return 1;
}
std::string Warrior::getTextureName() const {
	std::string name = this->getBaseTextureName();
	name = name + " " + this->currentAnimation;
	name = name + " " + this->currentDirection;
	name = name + std::to_string(this->getCurrentAnimationState().frame);

	return name;
}
uint32_t Warrior::getAnimationNumber(const std::string &type, const std::string &direction) const {
	if (type == "talking") {
		if (direction == "n" or direction == "s" or direction == "w" or direction == "e" or direction == "nw" or direction == "ne" or direction == "sw" or direction == "se") {
			return this->getTalkingAnimationsNumberInSet();
		}
		return 0;
	}
	if (type == "running") {
		if (direction == "n" or direction == "s" or direction == "w" or direction == "e") {
			return this->getRunningAnimationsNumberInSet();
		}
		return 0;
	}
	if (type == "been hit") {
		if (direction == "n" or direction == "s" or direction == "w" or direction == "e" or direction == "nw" or direction == "ne" or direction == "sw" or direction == "se") {
			return this->getBeenHitAnimationsNumberInSet();
		}
		return 0;
	}
	if (type == "tipping over") {
		if (direction == "n" or direction == "s" or direction == "w" or direction == "e" or direction == "nw" or direction == "ne" or direction == "sw" or direction == "se") {
			return this->getTippingOverAnimationsNumberInSet();
		}
		return 0;
	}
	return 0;
}
uint32_t Warrior::getCurrentAnimationMs() const {
    if (this->currentAnimation == "talking") {
        return 750;
    }
    if (this->currentAnimation == "running") {
        return 500 * (1 + 0.75f * this->isVehicle());
    }
    if (this->currentAnimation == "been hit") {
        return 500;
    }
    if (this->currentAnimation == "tipping over") {
        return 600;
    }
    return 0;
}
bool Warrior::canStay(MapState *state, uint32_t newX, uint32_t newY) const {
	sf::IntRect thisRect;
	thisRect.left = newX;
	thisRect.top = newY;
	thisRect.width = this->getSX();
	thisRect.height = this->getSY();

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalGOs(); i = i + 1) {
        GO* go = state->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY);
		if (go->exist() and !go->warriorCanStay(this)) {
			sf::IntRect goRect;
			goRect.left = go->getX();
			goRect.top = go->getY();
			goRect.width = go->getSX();
			goRect.height = go->getSY();
			if (thisRect.intersects(goRect)) {
				return false;
			}
		}
	}

	return true;
}
bool Warrior::warriorCanStay(const Warrior *w) const {
	return false;
}
uint32_t Warrior::getWarriorMovementCost(const Warrior *w) const {
	if (this->getPlayerId() == w->getPlayerId()) {
		return 1;
	}
    return 10000;
}
DrawingPriority Warrior::getDrawingPriority() const {
    if (this->isFlying()) {
        return GO::DRAWING_PRIORITY_WARRIOR_FLYING;
    }
    if (this->isVehicle()) {
        return GO::DRAWING_PRIORITY_WARRIOR_VEHICLE;
    }
    return GO::DRAWING_PRIORITY_WARRIOR_DEFAULT;
}
ClickPriority Warrior::getClickPriority() const {
    return GO::CLICK_PRIORITY_WARRIOR;
}
NewMoveMainPriority Warrior::getNewMoveMainPriority() const {
    return GO::NEW_MOVE_MAIN_PRIORITY_WARRIOR;
}
Events Warrior::processCurrentAnimation(MapState *state) {
    if (currentAnimation == "running") {
        return this->processRunningAnimation(state);
    }
    if (currentAnimation == "been hit") {
        return this->processBeenHitAnimation();
    }
    if (currentAnimation == "tipping over") {
        return this->processTippingOverAnimation();
    }
    return Events();
}
void Warrior::startAnimation(const std::string& type) {
    this->animationClock.restart();
    this->currentAnimation = type;
}
std::string Warrior::getCurrentAnimation() const {
    return this->currentAnimation;
}
AnimationState Warrior::getCurrentAnimationState() const {
    uint32_t ms = this->animationClock.getMS();
    uint32_t animationNumber = this->getAnimationNumber(this->currentAnimation, this->currentDirection);
    uint32_t msForFrame = this->getCurrentAnimationMs() / animationNumber;
    uint32_t currentFrame = ms / msForFrame;

    if (currentFrame < animationNumber) {
        return {currentFrame, false};
    }

    if (this->currentAnimation == "talking") { // Talking animation never finishes, but is not suspending
        if (this->delayBetweenTalkingAnimations() and currentFrame / animationNumber % 2 == 1) {
            return {0, false};
        }
        return {currentFrame % animationNumber, false};
    }

    return {animationNumber - 1, true};
}
Defence Warrior::getDefence() const {
    return (1 + Parameters::get()->getDouble("rage_mode_defence_bonus") * (this->inRage())) * this->getBaseDefence();
}
std::vector<std::tuple<uint32_t, uint32_t>> Warrior::getMoves(MapState *state) {
    return this->buildMovementGraph(state).getMoves(this->getX(), this->getY(), this->movementPoints.value());
}
Events Warrior::getMoveHighlightionEvent(MapState *state) {
    Events event;

    std::vector<std::tuple<uint32_t, uint32_t>> moves = this->getMoves(state);
    for (uint32_t i = 0; i < moves.size(); i = i + 1) {
        std::tuple<uint32_t, uint32_t> move = moves.at(i);
        event.add(std::make_shared<SetHighlightEvent>(HighlightColors::get()->getWarriorMovementColor(this->getPlayerId()), std::get<0>(move), std::get<1>(move)));
    }

    std::vector<SpecialMove> specialMoves = this->getSpecialMoves(state);
    for (uint32_t i = 0; i < specialMoves.size(); i = i + 1) {
        SpecialMove specMove = specialMoves.at(i);
        event.add(std::make_shared<SetHighlightEvent>(specMove.color, specMove.targetX, specMove.targetY));
    }

    return event;
}
bool Warrior::isVehicle() const {
    return false;
}
bool Warrior::isFlying() const {
    return false;
}
bool Warrior::delayBetweenTalkingAnimations() const {
    return true;
}
void Warrior::setDirection(const std::string &newDirection) {
    this->currentDirection = newDirection;
}
std::shared_ptr<sf::Drawable> Warrior::getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const {
    sf::Sprite sprite;
    sprite.setTexture(*Textures::get()->get("hand"));
    sprite.setPosition(mouseX, mouseY);
    return std::make_shared<sf::Sprite>(sprite);
}
void Warrior::newFrame(MapState *state, uint32_t currentPlayerId) {
    if (this->exist() and this->newFrameUpdateTimer.ready()) {
        this->newFrameUpdateTimer.reset();
        if (this->getPlayerId() == currentPlayerId) {
            this->enemyMove = false;
            this->hasSpecialMoves = !this->getSpecialMoves(state).empty();
        }
        else {
            this->enemyMove = true;
        }
    }
}
Events Warrior::unselect(MapState *state, uint32_t x, uint32_t y, uint8_t button) {
    Events events;
    events.add(std::make_shared<UnselectEvent>());
    events.add(std::make_shared<ResetHighlightEvent>());
    events.add(std::make_shared<EnableCursorEvent>());

    Events specialMoveEvents;
    if (button == sf::Mouse::Button::Left) {
        specialMoveEvents = this->handleSpecialMove(state, x, y);
        events = events + specialMoveEvents;
    }

    if ((button == sf::Mouse::Button::Left and specialMoveEvents.empty()) or button == sf::Mouse::Button::Right) {
        try {
            Move move = this->getMove(state, x, y);
            this->currentMovement = move.route;
            this->startAnimation("running");
            this->currentDirection = move.route.front();
            this->movementPoints = this->movementPoints.value() - move.dst;
            events.add(std::make_shared<CreateAnimationEvent>(SuspendingAnimation(this)));
        }
        catch (MoveDoesNotExist&) {

        }
    }

    return events;
}
Move Warrior::getMove(MapState *state, uint32_t x2, uint32_t y2) {
    return this->buildMovementGraph(state).getMove(this->getX(), this->getY(), x2, y2, this->movementPoints.value());
}
MovementGraph Warrior::buildMovementGraph(MapState *state) {
    MovementGraph g(state->getMapSizePtr()->getWidth(), state->getMapSizePtr()->getHeight());

    if (!this->movementPoints.has_value()) {
        this->movementPoints = this->getMovementPoints();
    }
    
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalGOs(); i = i + 1) {
        GO* go = state->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY);
        if (go->exist()) {
            for (uint32_t x = go->getX(); x < go->getX() + go->getSX(); x = x + 1) {
                for (uint32_t y = go->getY(); y < go->getY() + go->getSY(); y = y + 1) {
                    if (!go->warriorCanStay(this)) {
                        g.setCantStay(x, y);
                    }
                    if (go->getWarriorMovementCost(this) != 1) {
                        g.updateMovementCost(x, y, go->getWarriorMovementCost(this));
                    }
                }
            }
        }
    }

    return g;
}
Events Warrior::processRunningAnimation(MapState *state) {
    Events events;

    if (this->isVehicle()) {

    }
    else {
        if (this->isFlying()) {
            if (this->footstepsClock.getMS() >= 500) {
                events.add(std::make_shared<PlaySoundEvent>("wings" + std::to_string(GlobalRandomGenerator32::get()->gen() % TOTAL_WINGS + 1), true));
                this->footstepsClock.restart();
            }
        }
        else {
            if (this->footstepsClock.getMS() >= 250) {
                events.add(std::make_shared<PlaySoundEvent>("footsteps" + std::to_string(GlobalRandomGenerator32::get()->gen() % TOTAL_FOOTSTEPS + 1), true));
                this->footstepsClock.restart();
            }
        }
    }

    if (this->getCurrentAnimationState().finished) {
        if (this->currentMovement.front() == "e") {
            this->setX(this->getX() + 1);
        }
        else if (this->currentMovement.front() == "w") {
            this->setX(this->getX() - 1);
        }
        else if (this->currentMovement.front() == "n") {
            this->setY(this->getY() - 1);
        }
        else if (this->currentMovement.front() == "s") {
            this->setY(this->getY() + 1);
        }
        this->currentMovement.pop();
        events.add(std::make_shared<CloseAnimationEvent>());

        if (this->currentMovement.empty()) {
            this->startAnimation("talking");
        }
        else {
            this->startAnimation("running");
            this->currentDirection = this->currentMovement.front();
            events.add(std::make_shared<CreateAnimationEvent>(SuspendingAnimation(this)));
        }
    }

    return events;
}
Events Warrior::processBeenHitAnimation() {
    Events events;

    if (this->getCurrentAnimationState().finished) {
        this->startAnimation("talking");
        events.add(std::make_shared<CloseAnimationEvent>());
    }

    return events;
}
Events Warrior::processTippingOverAnimation() {
    Events events;

    if (this->getCurrentAnimationState().finished) {
        events.add(std::make_shared<CloseAnimationEvent>());
        events.add(std::make_shared<SubHpEvent>(this, 1));
    }

    return events;
}
float Warrior::getOffsetX() const {
    if (this->currentMovement.empty()) {
        return 0;
    }
    if (this->currentMovement.front() == "w") {
        return -this->getOffset();
    }
    if (this->currentMovement.front() == "e") {
        return this->getOffset();
    }
    return 0;
}
float Warrior::getOffsetY() const {
    if (this->currentMovement.empty()) {
        return 0;
    }
    if (this->currentMovement.front() == "n") {
        return -this->getOffset();
    }
    if (this->currentMovement.front() == "s") {
        return this->getOffset();
    }
    return 0;
}
float Warrior::getOffset() const {
    return 64 * (float)this->animationClock.getMS() / (float)this->getCurrentAnimationMs();
}
sf::Color Warrior::getTextureColor() const {
    if (this->inRage()) {
        return sf::Color(150, 0, 255);
    }
    return this->Unit::getTextureColor();
}
HorizontalSelectionWindowComponent Warrior::getRageModeComponent() const {
    return {
          "rage_spell",
        StringLcl("{rage_spell_description}") + "\n" +
        StringLcl("{moves_left}") + std::to_string(this->rageModeMovesLeft),
        false,
        Events()
    };
}
HorizontalSelectionWindowComponent Warrior::getKillComponent() {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events killNextTurnEvent = clickSoundEvent;
    killNextTurnEvent.add(std::make_shared<KillNextTurnEvent>(this));

    std::shared_ptr<WindowTwoButtons> verifyWindow = std::make_shared<WindowTwoButtons>(StringLcl("{verify_kill}"), StringLcl("{yes}"), StringLcl("{no}"), killNextTurnEvent, clickSoundEvent);

    Events killNextTurnEventVerify = clickSoundEvent;
    killNextTurnEventVerify.add(std::make_shared<ResetHighlightEvent>());
    killNextTurnEventVerify.add(std::make_shared<CreateEEvent>(verifyWindow));

    return {
          "skull",
        StringLcl("{kill}"),
        true,
        killNextTurnEventVerify
    };
}
HorizontalSelectionWindowComponent Warrior::getRevertKillComponent() {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events revertKillNextTurnEvent = clickSoundEvent;
    revertKillNextTurnEvent.add(std::make_shared<RevertKillNextTurnEvent>(this));

    return {
          "skull",
        StringLcl("{revert_kill}"),
        true,
        revertKillNextTurnEvent
    };
}
HorizontalSelectionWindowComponent Warrior::getWarriorInfoComponent() const {
    return {
          "helmet",
        StringLcl("{hp}") + std::to_string(this->getHP()) + " / " + std::to_string(this->getMaxHP()) + " (" + this->getDefence().getReadable() + ")\n" +
        this->getSpecialInfoString() + "\n" +
        StringLcl("{movement_points}") + std::to_string(this->movementPoints.value_or(this->getMovementPoints())) + " / " + std::to_string(this->getMovementPoints()) + "\n" +
        StringLcl("{population}") + std::to_string(this->getPopulation()),
        false,
        Events()
    };
}
HorizontalSelectionWindowComponent Warrior::getBlockingBuildingComponent() const {
    return {
          "destroyed_icon",
        StringLcl("{blocking_building}"),
        false,
        Events()
    };
}
HorizontalSelectionWindowComponent Warrior::getWarriorOfEnemyComponent() const {
    return {
          "lord_icon",
        StringLcl("{warrior_of_enemy}"),
        false,
        Events()
    };
}
Events Warrior::getSelectionWindow(bool own) {
    Events events;

    std::vector<HorizontalSelectionWindowComponent> components;
    components.push_back(this->getExitComponent());
    if (!own) {
        components.push_back(this->getWarriorOfEnemyComponent());
    }
    components.push_back(this->getDescriptionComponent());
    components.push_back(this->getWarriorInfoComponent());
    if (this->blockBuildingAbility()) {
        components.push_back(this->getBlockingBuildingComponent());
    }
    if (this->inRage()) {
        components.push_back(this->getRageModeComponent());
    }
    if (own) {
        if (this->toKill) {
            components.push_back(this->getRevertKillComponent());
        }
        else {
            components.push_back(this->getKillComponent());
        }
        events.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
    }
    else {
        events.add(std::make_shared<PlaySoundEvent>("click"));
    }

    std::shared_ptr<HorizontalSelectionWindow> w = std::make_shared<HorizontalSelectionWindow>(components);
    events.add(std::make_shared<CreateEEvent>(w));

    return events;
}
Events Warrior::getResponse(MapState *state, uint32_t playerId, uint32_t button) {
	if (!this->exist()) {
		return Events();
	}

    if (button == sf::Mouse::Button::Right or (button == sf::Mouse::Button::Left and !this->belongTo(playerId))) {
        return this->getMoveHighlightionEvent(state) + this->getSelectionWindow(this->belongTo(playerId));
    }

    if (!this->belongTo(playerId)) {
        return Events();
    }

	Events response;

	response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
    if (this->movementPoints.value_or(this->getMovementPoints()) > 0 or this->hasSpecialMoves) {
        Events selectThisEvent = this->getMoveHighlightionEvent(state);
        selectThisEvent.add(std::make_shared<SelectEvent>(this));
        selectThisEvent.add(std::make_shared<DisableCursorEvent>());

        if (FirstTimeTipsTable::get()->wasDisplayed("warrior_selection_guide")) {
            response = response + selectThisEvent;
        }
        else {
            FirstTimeTipsTable::get()->markAsDisplayed("warrior_selection_guide");

            selectThisEvent.add(std::make_shared<PlaySoundEvent>("click"));

            std::shared_ptr<WindowButton> warriorSelectionGuide = std::make_shared<WindowButton>(StringLcl("{warrior_selection_guide}"), StringLcl("{OK}"), selectThisEvent);
            response.add(std::make_shared<CreateEEvent>(warriorSelectionGuide));
        }
    }
    else {
        Events clickSoundEvent;
        clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

        std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{no_more_movement_points}"), StringLcl("{OK}"), clickSoundEvent);
        response.add(std::make_shared<CreateEEvent>(w));
    }
	return response;
}
std::shared_ptr<PlayerPointer> Warrior::getPlayerPointer() const {
    uint8_t state;
    if (this->enemyMove) {
        state = WarriorPlayerPointer::ENEMY_MOVE;
    }
    else {
        if (this->movementPoints.value_or(1) or this->hasSpecialMoves) {
            state = WarriorPlayerPointer::HAS_MOVES;
        }
        else {
            state = WarriorPlayerPointer::DOES_NOT_HAVE_MOVES;
        }
    }
    return std::make_shared<WarriorPlayerPointer>(this->getXInPixels(), this->getYInPixels(), state);
}
void Warrior::drawHPPointer(sf::RenderTarget& target, sf::RenderStates states) const {
    WarriorHPPointer pointer(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY(), this->getHP(), this->getMaxHP());
    target.draw(pointer, states);
}


BOOST_CLASS_EXPORT_IMPLEMENT(Warrior)