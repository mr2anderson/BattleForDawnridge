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
#include "StartWarriorClickAnimationEvent.hpp"
#include "ChangeHighlightEvent.hpp"
#include "RefreshMovementPointsEvent.hpp"
#include "ColorTheme.hpp"
#include "DisableCursorEvent.hpp"
#include "MoveDoesNotExist.hpp"
#include "CreateAnimationEvent.hpp"
#include "CloseAnimationEvent.hpp"
#include "WindowButton.hpp"
#include "Texts.hpp"
#include "CreateEEvent.hpp"
#include "MovementPointsPointer.hpp"
#include "PlayerPointerCircle.hpp"


Warrior::Warrior() {
    this->currentDirection = "e";
}
Warrior::Warrior(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
	Unit(x, y, std::nullopt, playerId, units) {
	this->movementPoints = std::nullopt;
	this->go = go;
	this->mapW = mapW;
	this->mapH = mapH;
    this->currentDirection = "e";
    this->startAnimation("talking");
}
void Warrior::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    this->Unit::draw(target, states);
}
Events Warrior::newMove(uint32_t currentPlayerId) {
	if (!this->exist() or !this->belongTo(currentPlayerId)) {
		Events events;
		events.add(std::make_shared<RefreshMovementPointsEvent>(this));
		return events;
	}
	return Events();
}
void Warrior::refreshMovementPoints() {
	this->movementPoints = this->getMovementPoints();
}
void Warrior::startClickAnimation() {
    this->startAnimation("talking");
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
        return 400;
    }
    if (this->currentAnimation == "attack") {
        return 600;
    }
    if (this->currentAnimation == "been hit") {
        return 300;
    }
    if (this->currentAnimation == "tipping over") {
        return 400;
    }
    return 0;
}
bool Warrior::canStay(uint32_t newX, uint32_t newY) const {
	sf::IntRect thisRect;
	thisRect.left = newX;
	thisRect.top = newY;
	thisRect.width = this->getSX();
	thisRect.height = this->getSY();

	for (uint32_t i = 0; i < this->go->size(); i = i + 1) {
		GO* o = this->go->at(i);
		if (o->exist() and !o->warriorCanStay(this->getPlayerId())) {
			sf::IntRect goRect;
			goRect.left = o->getX();
			goRect.top = o->getY();
			goRect.width = o->getSX();
			goRect.height = o->getSY();
			if (thisRect.intersects(goRect)) {
				return false;
			}
		}
	}

	return true;
}
uint32_t Warrior::getMovementCost(uint32_t newX, uint32_t newY) const {
	sf::IntRect thisRect;
	thisRect.left = newX;
	thisRect.top = newY;
	thisRect.width = this->getSX();
	thisRect.height = this->getSY();

	uint32_t cost = 1;

	for (uint32_t i = 0; i < this->go->size(); i = i + 1) {
		GO* o = this->go->at(i);
		if (o->exist()) {
			sf::IntRect goRect;
			goRect.left = o->getX();
			goRect.top = o->getY();
			goRect.width = o->getSX();
			goRect.height = o->getSY();
			if (thisRect.intersects(goRect)) {
				cost = std::max(cost, o->getWarriorMovementCost(this->getPlayerId()));
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
    return Events();
}
bool Warrior::highDrawingPriority() const {
	return true;
}
bool Warrior::highClickPriority() const {
	return true;
}
std::string Warrior::getSelectableTextureName() const {
	return "hand";
}
Events Warrior::unselect(uint32_t x, uint32_t y) {
	Events events = this->Selectable::unselect(x, y) + this->getMoveHighlightionEvent();

    try {
        Move move = this->getMove(x, y);
        events.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
        this->currentMovement = move.route;
        this->startAnimation("running");
        this->currentDirection = move.route.front();
        this->movementPoints = this->movementPoints.value() - move.dst;
        events.add(std::make_shared<CreateAnimationEvent>(Animation(this)));
    }
	catch (MoveDoesNotExist&) {

    }

    return events;
}
Events Warrior::unselect() {
	return this->Selectable::unselect() + this->getMoveHighlightionEvent();
}
Events Warrior::getMoveHighlightionEvent() {
	Events event;

	std::vector<std::tuple<uint32_t, uint32_t>> moves = this->getMoves();

	for (uint32_t i = 0; i < moves.size(); i = i + 1) {
		std::tuple<uint32_t, uint32_t> move = moves.at(i);
		event.add(std::make_shared<ChangeHighlightEvent>(this, COLOR_THEME::CELL_COLOR_HIGHLIGHTED_GREEN, std::get<0>(move), std::get<1>(move), this->getSX(), this->getSY()));
	}

	return event;
}
std::vector<std::tuple<uint32_t, uint32_t>> Warrior::getMoves() {
	return this->buildMovementGraph().getMoves(this->getX(), this->getY(), this->movementPoints.value());
}
Move Warrior::getMove(uint32_t x2, uint32_t y2) {
    return this->buildMovementGraph().getMove(this->getX(), this->getY(), x2, y2, this->movementPoints.value());
}
MovementGraph Warrior::buildMovementGraph() {
    MovementGraph g(this->mapW, this->mapH);

    if (!this->movementPoints.has_value()) {
        this->movementPoints = this->getMovementPoints();
    }

    uint32_t xMin;
    if (this->movementPoints >= this->getX()) {
        xMin = 0;
    }
    else {
        xMin = this->getX() - this->movementPoints.value();
    }

    uint32_t yMin;
    if (this->movementPoints >= this->getY()) {
        yMin = 0;
    }
    else {
        yMin = this->getY() - this->movementPoints.value();
    }

    uint32_t xMax = std::min(this->mapW - this->getSX(), this->getX() + this->movementPoints.value());
    uint32_t yMax = std::min(this->mapH - this->getSY(), this->getY() + this->movementPoints.value());

    for (uint32_t x = xMin; x <= xMax; x = x + 1) {
        for (uint32_t y = yMin; y <= yMax; y = y + 1) {
            g.set(x, y, this->canStay(x, y), this->getMovementCost(x, y));
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
            events.add(std::make_shared<CreateAnimationEvent>(Animation(this)));
        }
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
void Warrior::startAnimation(const std::string &type) {
	this->animationClock.restart();
	this->currentAnimation = type;
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
Events Warrior::getGameObjectResponse(uint32_t playerId) {
	if (!this->exist()) {
		return Events();
	}
	if (!this->belongTo(playerId)) {
		return Events();
	}
	Events response;
	response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	response.add(std::make_shared<StartWarriorClickAnimationEvent>(this));
    if (this->movementPoints.value_or(this->getMovementPoints()) > 0) {
        response = response + this->getMoveHighlightionEvent();
        response.add(std::make_shared<SelectEvent>(this));
        response.add(std::make_shared<DisableCursorEvent>());
    }
    else {
        Events clickSoundEvent;
        clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

        std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("no_more_movement_points"), *Texts::get()->get("OK"), clickSoundEvent);
        response.add(std::make_shared<CreateEEvent>(w));
    }
	return response;
}
std::shared_ptr<HPPointer> Warrior::getHPPointer() const {
    return std::make_shared<HPPointer>(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY(), HPPointer::ORIENTATION::LEFT_UP);
}
std::shared_ptr<PlayerPointer> Warrior::getPlayerPointer() const {
    if (this->movementPoints.value_or(this->getMovementPoints()) > 0) {
        return std::make_shared<MovementPointsPointer>(this->getXInPixels(), this->getYInPixels(), this->movementPoints.value_or(this->getMovementPoints()));
    }
    return std::make_shared<PlayerPointerCircle>(this->getXInPixels(), this->getYInPixels(), 1, 1, PlayerPointerCircle::ORIENTATION::LEFT_UP);
}