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


#include "Warrior.hpp"
#include "SelectEvent.hpp"
#include "SetHighlightEvent.hpp"
#include "RefreshMovementPointsEvent.hpp"
#include "ColorTheme.hpp"
#include "DisableCursorEvent.hpp"
#include "MoveDoesNotExist.hpp"
#include "CreateAnimationEvent.hpp"
#include "CloseAnimationEvent.hpp"
#include "WindowButton.hpp"
#include "Texts.hpp"
#include "CreateEEvent.hpp"
#include "WarriorPlayerPointer.hpp"
#include "PlayerPointerCircle.hpp"
#include "HPFlyingE.hpp"
#include "SubHpEvent.hpp"
#include "Textures.hpp"
#include "ChangeWarriorDirectionEvent.hpp"
#include "FirstTimeTipsTable.hpp"
#include "StartWarriorAnimationEvent.hpp"
#include "HPPointer.hpp"
#include "UnselectEvent.hpp"
#include "ResetHighlightEvent.hpp"
#include "EnableCursorEvent.hpp"
#include <iostream>


Warrior::Warrior() {
    this->currentDirection = "e";
    this->startAnimation("talking");
}
Warrior::Warrior(uint32_t x, uint32_t y, uint32_t playerId) :
	Unit(x, y, std::nullopt, playerId) {
	this->movementPoints = std::nullopt;
    this->currentDirection = "e";
    this->startAnimation("talking");
}
void Warrior::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    this->Unit::draw(target, states);
    if (this->exist()) {
        this->drawHPPointer(target, states);
    }
}
Events Warrior::hit(Damage d, const std::optional<std::string> &direction) {
    uint32_t dPoints = d.getHpLoss(this->getDefence());
    uint32_t hpPointsAfterOperation;
    if (dPoints >= this->getHP()) {
        hpPointsAfterOperation = 0;
    }
    else {
        hpPointsAfterOperation = this->getHP() - dPoints;
    }

    Events response;

    if (direction.has_value()) {
        response.add(std::make_shared<ChangeWarriorDirectionEvent>(this, direction.value()));
    }

    response.add(std::make_shared<PlaySoundEvent>("ouch"));

    if (hpPointsAfterOperation == 0) {
        response.add(std::make_shared<StartWarriorAnimationEvent>(this, "tipping over"));
    }
    else {
        response.add(std::make_shared<StartWarriorAnimationEvent>(this, "been hit"));
    }

    std::shared_ptr<HPFlyingE> hpFlyingE = std::make_shared<HPFlyingE>(dPoints, false, this->getX(), this->getY(), this->getSX(), this->getSY());
    response.add(std::make_shared<CreateEEvent>(hpFlyingE));

    response.add(std::make_shared<SubHpEvent>(this, this->getHP() - (hpPointsAfterOperation + 1 * (hpPointsAfterOperation == 0))));

    response.add(std::make_shared<CreateAnimationEvent>(SuspendingAnimation(this)));

    return response;
}
void Warrior::changeDirection(const std::string& newDirection) {
    this->currentDirection = newDirection;
}
Events Warrior::newMove(MapState *state, uint32_t currentPlayerId) {
	if (this->exist() and this->belongTo(currentPlayerId)) {
        Events events;
		events.add(std::make_shared<RefreshMovementPointsEvent>(this));
		return events;
	}
	return Events();
}
void Warrior::refreshMovementPoints() {
	this->movementPoints = this->getMovementPoints();
}
uint32_t Warrior::getSX() const {
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
	if (type == "attack") {
		if (direction == "n" or direction == "s" or direction == "w" or direction == "e") {
			return this->getAttackAnimationsNumberInSet();
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
        return 500;
    }
    if (this->currentAnimation == "attack") {
        return 600;
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
        GO* go = state->getCollectionsPtr()->getGO(i);
		if (go->exist() and !go->warriorCanStay(this->getPlayerId())) {
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
uint32_t Warrior::getMovementCost(MapState *state, uint32_t newX, uint32_t newY) const {
	sf::IntRect thisRect;
	thisRect.left = newX;
	thisRect.top = newY;
	thisRect.width = this->getSX();
	thisRect.height = this->getSY();

	uint32_t cost = 1;

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalGOs(); i = i + 1) {
        GO* go = state->getCollectionsPtr()->getGO(i);
		if (go->exist()) {
			sf::IntRect goRect;
			goRect.left = go->getX();
			goRect.top = go->getY();
			goRect.width = go->getSX();
			goRect.height = go->getSY();
			if (thisRect.intersects(goRect)) {
				cost = std::max(cost, go->getWarriorMovementCost(this->getPlayerId()));
			}
		}
	}

	return cost;
}
bool Warrior::warriorCanStay(uint32_t warriorPlayerId) const {
	return false;
}
uint32_t Warrior::getWarriorMovementCost(uint32_t warriorPlayerId) const {
	if (this->getPlayerId() == warriorPlayerId) {
		return 1;
	}
	return WARRIOR_MOVEMENT_FORBIDDEN;
}
Events Warrior::processCurrentAnimation() {
    if (currentAnimation == "running") {
        return this->processRunningAnimation();
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
bool Warrior::highDrawingPriority() const {
	return true;
}
bool Warrior::highClickPriority() const {
	return true;
}
std::shared_ptr<sf::Drawable> Warrior::getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const {
    sf::Sprite sprite;
    sprite.setTexture(*Textures::get()->get("hand"));
    sprite.setPosition(mouseX, mouseY);
    return std::make_shared<sf::Sprite>(sprite);
}
Events Warrior::unselect(MapState *state, uint32_t x, uint32_t y, uint8_t button) {
    Events events;
    events.add(std::make_shared<UnselectEvent>());
    events.add(std::make_shared<ResetHighlightEvent>());
    events.add(std::make_shared<EnableCursorEvent>());

    try {
        Move move = this->getMove(state, x, y);
        events.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
        this->currentMovement = move.route;
        this->startAnimation("running");
        this->currentDirection = move.route.front();
        this->movementPoints = this->movementPoints.value() - move.dst;
        events.add(std::make_shared<CreateAnimationEvent>(SuspendingAnimation(this)));
    }
	catch (MoveDoesNotExist&) {

    }

    return events;
}
Events Warrior::getMoveHighlightionEvent(MapState *state) {
	Events event;

	std::vector<std::tuple<uint32_t, uint32_t>> moves = this->getMoves(state);

	for (uint32_t i = 0; i < moves.size(); i = i + 1) {
		std::tuple<uint32_t, uint32_t> move = moves.at(i);
		event.add(std::make_shared<SetHighlightEvent>(COLOR_THEME::CELL_COLOR_HIGHLIGHTED_GREEN, std::get<0>(move), std::get<1>(move), this->getSX(), this->getSY()));
	}

	return event;
}
std::vector<std::tuple<uint32_t, uint32_t>> Warrior::getMoves(MapState *state) {
	return this->buildMovementGraph(state).getMoves(this->getX(), this->getY(), this->movementPoints.value());
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
        GO* go = state->getCollectionsPtr()->getGO(i);
        if (go->exist()) {
            for (uint32_t x = go->getX(); x < go->getX() + go->getSX(); x = x + 1) {
                for (uint32_t y = go->getY(); y < go->getY() + go->getSY(); y = y + 1) {
                    if (!go->warriorCanStay(this->getPlayerId())) {
                        g.setCantStay(x, y);
                    }
                    if (go->getWarriorMovementCost(this->getPlayerId()) != 1) {
                        g.updateMovementCost(x, y, go->getWarriorMovementCost(this->getPlayerId()));
                    }
                }
            }
        }
    }

    return g;
}
Events Warrior::processRunningAnimation() {
    Events events;

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
    return 64 * (float)this->animationClock.getElapsedTime().asMilliseconds() / (float)this->getCurrentAnimationMs();
}
HorizontalSelectionWindowComponent Warrior::getWarriorInfoComponent() const {
    return {
        "helmet",
        *Texts::get()->get("hp") + std::to_wstring(this->getHP()) + L" / " + std::to_wstring(this->getMaxHP()) + L" (" + this->getDefence().getReadable() + L")\n" +
        *Texts::get()->get("damage") + this->getDamage().getReadable() + L"\n" +
        *Texts::get()->get("population") + std::to_wstring(this->getPopulation()),
        false,
        Events()
    };
}
Events Warrior::getSelectionWindow() const {
    std::vector<HorizontalSelectionWindowComponent> components;
    components.push_back(this->getExitComponent());
    components.push_back(this->getDescriptionComponent());
    components.push_back(this->getWarriorInfoComponent());

    std::shared_ptr<HorizontalSelectionWindow> w = std::make_shared<HorizontalSelectionWindow>(components);

    Events events;
    events.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
    events.add(std::make_shared<CreateEEvent>(w));

    return events;
}
AnimationState Warrior::getCurrentAnimationState() const {
    uint32_t ms = this->animationClock.getElapsedTime().asMilliseconds();
    uint32_t animationNumber = this->getAnimationNumber(this->currentAnimation, this->currentDirection);
    uint32_t msForFrame = this->getCurrentAnimationMs() / animationNumber;
    uint32_t currentFrame = ms / msForFrame;
    if (currentFrame > animationNumber - 1) {
        return {animationNumber - 1, true};
    }
    return {currentFrame, false};
}
Events Warrior::getResponse(MapState *state, uint32_t playerId, uint32_t button) {
	if (!this->exist()) {
		return Events();
	}
	if (!this->belongTo(playerId)) {
		return Events();
	}

    if (button == sf::Mouse::Button::Right) {
        return this->getSelectionWindow();
    }

	Events response;
	response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
    response.add(std::make_shared<StartWarriorAnimationEvent>(this, "talking"));
    if (this->movementPoints.value_or(this->getMovementPoints()) > 0) {
        Events selectThisEvent = this->getMoveHighlightionEvent(state);
        selectThisEvent.add(std::make_shared<SelectEvent>(this));
        selectThisEvent.add(std::make_shared<DisableCursorEvent>());

        if (FirstTimeTipsTable::get()->wasDisplayed("warrior_selection_guide")) {
            response = response + selectThisEvent;
        }
        else {
            FirstTimeTipsTable::get()->markAsDisplayed("warrior_selection_guide");

            selectThisEvent.add(std::make_shared<PlaySoundEvent>("click"));

            std::shared_ptr<WindowButton> warriorSelectionGuide = std::make_shared<WindowButton>(*Texts::get()->get("warrior_selection_guide"), *Texts::get()->get("OK"), selectThisEvent);
            response.add(std::make_shared<CreateEEvent>(warriorSelectionGuide));
        }
    }
    else {
        Events clickSoundEvent;
        clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

        std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("no_more_movement_points"), *Texts::get()->get("OK"), clickSoundEvent);
        response.add(std::make_shared<CreateEEvent>(w));
    }
	return response;
}
std::shared_ptr<PlayerPointer> Warrior::getPlayerPointer() const {
    return std::make_shared<WarriorPlayerPointer>(this->getXInPixels(), this->getYInPixels(), this->movementPoints.value_or(this->getMovementPoints()));
}
void Warrior::drawHPPointer(sf::RenderTarget& target, sf::RenderStates states) const {
    HPPointer pointer(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY(), HPPointer::ORIENTATION::DOWN);
    pointer.setCurrent(this->getHP());
    pointer.setMax(this->getMaxHP());
    target.draw(pointer, states);
}