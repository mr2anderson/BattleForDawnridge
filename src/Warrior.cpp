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
#include "StartWarriorAnimationEvent.hpp"
#include "WarriorHPPointer.hpp"
#include "ResetHighlightEvent.hpp"
#include "KillNextTurnEvent.hpp"
#include "RevertKillNextTurnEvent.hpp"
#include "WindowTwoButtons.hpp"
#include "DecreaseRageModeMovesLeftEvent.hpp"
#include "Parameters.hpp"
#include "GlobalRandomGenerator32.hpp"
#include "FocusOnEvent.hpp"
#include "Building.hpp"
#include "IAreaControllerSpec.hpp"
#include "PlaySoundEvent.hpp"
#include "RefreshWasHealedStatusEvent.hpp"
#include "ColorTheme.hpp"
#include "ImageFlyingE.hpp"
#include "SetPoisonEffectEvent.hpp"
#include "ColorBlender.hpp"
#include "AddHpEvent.hpp"
#include "TextFlyingE.hpp"
#include "WipePoisonEffectEvent.hpp"
#include "SetInspiredEffectEvent.hpp"
#include "WipeInspiredEffectEvent.hpp"


const uint32_t Warrior::TOTAL_FOOTSTEPS = 10;
const uint32_t Warrior::TOTAL_WINGS = 3;


Warrior::Warrior() {
    this->defaultDirection = "e";
    this->startAnimation("talking");
}
Warrior::Warrior(uint32_t x, uint32_t y, uint32_t playerId) :
	Unit(x, y, boost::none, playerId) {
	this->movementPoints = boost::none;
    this->wasHealedThisMove = false;
    this->defaultDirection = "e";
    this->startAnimation("talking");
    this->toKill = false;
    this->rageModeMovesLeft = 0;
    this->poison = false;
    this->inspired = false;
    this->hasSpecialMoves = false;
    this->enemyMove = true;
}
void Warrior::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    this->Unit::draw(target, states);
    if (this->exist()) {
        this->drawHPPointer(target, states);
    }
}
StringLcl Warrior::getNameString() const {
    if (this->isVehicle()) {
        return {};
    }
    return this->name.toString();
}
Events Warrior::hit(Damage damage) {
    uint32_t d = damage.getHpLoss(this->getDefence());

    uint32_t hpPointsAfterOperation;
    if (d >= this->getHP()) {
        hpPointsAfterOperation = 0;
    }
    else {
        hpPointsAfterOperation = this->getHP() - d;
    }

    Events response;

    response.add(std::make_shared<FocusOnEvent>(this->getX(), this->getY(), this->getSX(), this->getSY()));

    bool mustBePoisoned = (hpPointsAfterOperation != 0 and damage.hasSpec(Damage::SPEC::POISON) and !this->isVehicle());

    if (mustBePoisoned) {
        response.add(std::make_shared<SetPoisonEffectEvent>(this->getThis<Warrior>()));

        response.add(std::make_shared<PlaySoundEvent>("poison"));

        response.add(std::make_shared<StartWarriorAnimationEvent>(this->getThis<Warrior>(), "been hit"));

        std::shared_ptr<ImageFlyingE> imageFlyingE = std::make_shared<ImageFlyingE>("poison_icon", this->getX(), this->getY(), this->getSX(), this->getSY());
        response.add(std::make_shared<CreateEEvent>(imageFlyingE));
    }
    else {
        response.add(std::make_shared<PlaySoundEvent>(this->getBeenHitSoundName()));

        if (hpPointsAfterOperation == 0) {
            response.add(std::make_shared<StartWarriorAnimationEvent>(this->getThis<Warrior>(), "tipping over"));
        }
        else {
            response.add(std::make_shared<StartWarriorAnimationEvent>(this->getThis<Warrior>(), "been hit"));
        }
    };

    std::shared_ptr<HPFlyingE> hpFlyingE = std::make_shared<HPFlyingE>(d, false, this->getX(), this->getY(), this->getSX(), this->getSY());
    response.add(std::make_shared<CreateEEvent>(hpFlyingE));

    response.add(std::make_shared<SubHpEvent>(this->getThis<Warrior>(), this->getHP() - hpPointsAfterOperation - 1 * (hpPointsAfterOperation == 0)));

    response.add(std::make_shared<CreateAnimationEvent>(SuspendingAnimation(this->getThis<Warrior>())));

    return response;
}
Events Warrior::inspire() {
    Events events;

    events.add(std::make_shared<FocusOnEvent>(this->getX(), this->getY(), this->getSX(), this->getSY()));
    events.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
    events.add(std::make_shared<SetInspiredEffectEvent>(this->getThis<Warrior>()));
    events.add(std::make_shared<CreateEEvent>(std::make_shared<TextFlyingE>(StringLcl::COLOR(sf::Color::Green) + StringLcl("{inspired_effect_short}"), this->getX(), this->getY(), this->getSX(), this->getSY())));

    return events;
}
Events Warrior::heal() {
    Events events;

    if (this->poison) {
        events.add(std::make_shared<FocusOnEvent>(this->getX(), this->getY(), this->getSX(), this->getSY()));
        events.add(std::make_shared<PlaySoundEvent>("heal"));
        events.add(std::make_shared<CreateEEvent>(std::make_shared<TextFlyingE>(StringLcl::COLOR(sf::Color::Green) + StringLcl("{poison_removed}"), this->getX(), this->getY(), this->getSX(), this->getSY())));
        events.add(std::make_shared<WipePoisonEffectEvent>(this->getThis<Warrior>()));
    }
    else {
        uint32_t toAdd = std::min(Parameters::get().getInt("healing_speed"), this->getMaxHP()- this->getHP());
        if (toAdd != 0) {
            events.add(std::make_shared<FocusOnEvent>(this->getX(), this->getY(), this->getSX(), this->getSY()));
            events.add(std::make_shared<PlaySoundEvent>("heal"));
            events.add(std::make_shared<CreateEEvent>(std::make_shared<HPFlyingE>(toAdd, true, this->getX(), this->getY(), this->getSX(), this->getSY())));
            events.add(std::make_shared<AddHpEvent>(this->getThis<HPGO>(), Parameters::get().getInt("healing_speed")));
        }
    }
    return events;
}
Events Warrior::killNextTurn() {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{will_be_killed}"), StringLcl("{OK}"), clickSoundEvent);

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
    this->rageModeMovesLeft = Parameters::get().getInt("rage_mode_lifetime");
}
void Warrior::decreaseRageModeMovesLeft() {
    this->rageModeMovesLeft = this->rageModeMovesLeft - 1;
}
bool Warrior::inRage() const {
    return (this->rageModeMovesLeft > 0);
}
void Warrior::changeDirection(const std::string& newDirection) {
    this->defaultDirection = newDirection;
}
Events Warrior::newMove(MapState *state, uint32_t currentPlayerId) {
	if (this->exist() and this->belongTo(currentPlayerId)) {
        Events events;

        if (this->inRage()) {
            events.add(std::make_shared<DecreaseRageModeMovesLeftEvent>(this->getThis<Warrior>()));
        }

        if (this->poison and this->getHP() != 1) {
            events = events + this->hit(Damage(std::min(this->getHP() - 1, Parameters::get().getInt("poison_damage")), Damage::TYPE::SERVICE));
        }

		return events;
	}
	return Events();
}
Events Warrior::endMove(MapState *state, uint32_t playerId) {
    if (this->exist() and this->belongTo(playerId)) {
        Events events;

        events.add(std::make_shared<RefreshWasHealedStatusEvent>(this->getThis<Warrior>()));
        events.add(std::make_shared<RefreshMovementPointsEvent>(this->getThis<Warrior>()));
        events.add(std::make_shared<WipeInspiredEffectEvent>(this->getThis<Warrior>()));

        if (this->toKill) {
            events = events + this->hit(Damage(this->getHP(), Damage::TYPE::SERVICE));
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
    if (this->currentAnimation == "running") {
        name = name + " " + this->currentMovement.at(std::min<uint32_t>(this->currentMovement.size() - 1, this->animationClock.getMS() / this->getCurrentAnimationMs()));
    }
    else {
        name = name + " " + this->defaultDirection;
    }
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
    boost::optional<SpecialAnimation> special = this->getSpecialAnimation();
    if (special.has_value()) {
        if (type == special->name) {
            if (direction == "n" or direction == "s" or direction == "w" or direction == "e") {
                return special->straightFrames;
            }
            if (direction == "nw" or direction == "ne" or direction == "sw" or direction == "se") {
                return special->obliquelyFrames;
            }
            return 0;
        }
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
    boost::optional<SpecialAnimation> special = this->getSpecialAnimation();
    if (special.has_value()) {
        if (this->currentAnimation == special->name) {
            return special->ms;
        }
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
        std::shared_ptr<GO> go = state->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY);
		if (go->exist() and !go->warriorCanStay(this->getThis<Warrior>())) {
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
bool Warrior::warriorCanStay(std::shared_ptr<const Warrior> w) const {
	return false;
}
uint32_t Warrior::getWarriorMovementCost(std::shared_ptr<Warrior> w) const {
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
    boost::optional<SpecialAnimation> special = this->getSpecialAnimation();
    if (special.has_value()) {
        if (currentAnimation == special->name) {
            return this->processSpecialAnimation();
        }
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
    uint32_t animationNumber = this->getAnimationNumber(this->currentAnimation, this->defaultDirection);
    uint32_t animationMs = this->getCurrentAnimationMs();
    uint32_t msForFrame = animationMs / animationNumber;
    uint32_t currentFrame = ms / msForFrame;

    if (currentFrame < animationNumber) {
        return { currentFrame, false };
    }

    if (this->currentAnimation == "talking") { // Talking animation never finishes, but is not suspending
        if (this->delayBetweenTalkingAnimations() and currentFrame / animationNumber % 2 == 1) {
            return {0, false};
        }
        return {currentFrame % animationNumber, false};
    }

    if (this->currentAnimation == "running") { // Time of running animation based on route length
        return { currentFrame % animationNumber, ms > animationMs * this->currentMovement.size()};
    }

    return {animationNumber - 1, true};
}
Defence Warrior::getDefence() const {
    return (1 + Parameters::get().getDouble("rage_mode_defence_bonus") * (this->inRage())) * this->getBaseDefence();
}
std::vector<std::tuple<uint32_t, uint32_t>> Warrior::getMoves(MapState *state) {
    return this->buildMovementGraph(state).getMoves(this->getX(), this->getY(), this->movementPoints.value());
}
Events Warrior::getMoveHighlightionEvent(MapState *state) {
    Events event;

    std::vector<std::tuple<uint32_t, uint32_t>> moves = this->getMoves(state);
    for (uint32_t i = 0; i < moves.size(); i = i + 1) {
        std::tuple<uint32_t, uint32_t> move = moves.at(i);
        event.add(std::make_shared<SetHighlightEvent>(HighlightColors::get().getWarriorMovementColor(this->getPlayerId()), std::get<0>(move), std::get<1>(move)));
    }

    std::vector<SpecialMove> specialMoves = this->getSpecialMoves(state);
    for (uint32_t i = 0; i < specialMoves.size(); i = i + 1) {
        SpecialMove specMove = specialMoves.at(i);
        event.add(std::make_shared<SetHighlightEvent>(specMove.color, specMove.targetX, specMove.targetY));
    }

    return event;
}
StringLcl Warrior::getDetailedDescription(MapState *state) const {
    int32_t timeBonusVal = state->getTimePtr()->getTimeMod().getPercentDelta(Player::IS_POSITIVE(this->getPlayerId()));
    StringLcl timeBonus;
    if (Player::IS_POSITIVE(this->getPlayerId())) {
        timeBonus = StringLcl("{positive}");
    }
    else {
        timeBonus = StringLcl("{negative}");
    }
    if (timeBonusVal >= 0) {
        timeBonus = timeBonus + " " + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_BEST) + "+" + std::to_string(timeBonusVal) + "%";
    }
    else {
        timeBonus = timeBonus + " " + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_WORST) + std::to_string(timeBonusVal) + "%";
    }
    StringLcl result = this->getNameString() + "\n\n" + this->Unit::getDetailedDescription(state) + "\n" +
                       StringLcl("{movement_points}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR(this->movementPoints.value_or(this->getMovementPoints()), this->getMovementPoints())) + std::to_string(this->movementPoints.value_or(this->getMovementPoints())) + "\n" +
                       StringLcl("{max_movement_points}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_BEST) + std::to_string(this->getMovementPoints()) + "\n\n" +
                       timeBonus + "\n\n";
    if (this->inRage()) {
        result = result + StringLcl("{in_rage}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->rageModeMovesLeft) + "\n\n";
    }
    if (this->poison) {
        result = result + StringLcl("{poison_effect_short}") + "\n\n";
    }
    if (this->inspired) {
        result = result + StringLcl("{inspired_effect_short}") + "\n\n";
    }
    result = result + this->getSpecialInfoString(state) + "\n";
    return result;
}
bool Warrior::wasHealed() const {
    return this->wasHealedThisMove;
}
void Warrior::refreshWasHealedStatus() {
    this->wasHealedThisMove = false;
}
void Warrior::setPoisonStatus() {
    this->poison = true;
}
void Warrior::wipePoisonStatus() {
    this->poison = false;
}
void Warrior::setInspiredStatus() {
    this->inspired = true;
}
void Warrior::wipeInspiredStatus() {
    this->inspired = false;
}
bool Warrior::isInspired() const {
    return this->inspired;
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
std::shared_ptr<sf::Drawable> Warrior::getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const {
    sf::Sprite sprite;
    sprite.setTexture(*Textures::get().get("hand"));
    sprite.setPosition(mouseX, mouseY);
    return std::make_shared<sf::Sprite>(sprite);
}
void Warrior::update(MapState *state, uint32_t currentPlayerId) {
    if (this->getPlayerId() == currentPlayerId) {
        this->enemyMove = false;
        this->hasSpecialMoves = !this->getSpecialMoves(state).empty();
    }
    else {
        this->enemyMove = true;
    }
}
Events Warrior::onUnselect(MapState *state, uint32_t x, uint32_t y, uint8_t button) {
    Events events;
    events.add(std::make_shared<ResetHighlightEvent>());

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
            this->defaultDirection = move.route.front();
            this->movementPoints = this->movementPoints.value() - move.dst;
            events.add(std::make_shared<CreateAnimationEvent>(SuspendingAnimation(this->getThis<Warrior>())));
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
        std::shared_ptr<GO> go = state->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY);
        if (go->exist()) {
            for (uint32_t x = go->getX(); x < go->getX() + go->getSX(); x = x + 1) {
                for (uint32_t y = go->getY(); y < go->getY() + go->getSY(); y = y + 1) {
                    if (!go->warriorCanStay(this->getThis<Warrior>())) {
                        g.setCantStay(x, y);
                    }
                    if (go->getWarriorMovementCost(this->getThis<Warrior>()) != 1) {
                        g.updateMovementCost(x, y, go->getWarriorMovementCost(this->getThis<Warrior>()));
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
        if (this->footstepsClock.getMS() >= 750) {
            events.add(std::make_shared<PlaySoundEvent>("vehicle", true));
            this->footstepsClock.restart();
        }
    }
    else if (this->isFlying()) {
        if (this->footstepsClock.getMS() >= 500) {
            events.add(std::make_shared<PlaySoundEvent>("wings" + std::to_string(GlobalRandomGenerator32::get().gen() % TOTAL_WINGS + 1), true));
            this->footstepsClock.restart();
        }
    }
    else {
        if (this->footstepsClock.getMS() >= 250) {
            events.add(std::make_shared<PlaySoundEvent>("footsteps" + std::to_string(GlobalRandomGenerator32::get().gen() % TOTAL_FOOTSTEPS + 1), true));
            this->footstepsClock.restart();
        }
    }

    if (this->getCurrentAnimationState().finished) {
        this->startAnimation("talking");
        for (auto& d : this->currentMovement) {
            if (d == "e") {
                this->setX(this->getX() + 1);
            }
            else if (d == "w") {
                this->setX(this->getX() - 1);
            }
            else if (d == "n") {
                this->setY(this->getY() - 1);
            }
            else if (d == "s") {
                this->setY(this->getY() + 1);
            }
        }
        this->defaultDirection = this->currentMovement.back();
        this->currentMovement.clear();
        events.add(std::make_shared<CloseAnimationEvent>());
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
        events.add(std::make_shared<SubHpEvent>(this->getThis<Warrior>(), 1));
    }

    return events;
}
float Warrior::getOffsetX() const {
    return this->getOffset("w", "e");
}
float Warrior::getOffsetY() const {
    return this->getOffset("n", "s");
}
float Warrior::getOffset(const std::string& toNeg, const std::string& toPos) const {
    uint32_t msLeft = this->animationClock.getMS();
    std::string partial;
    int32_t offset = 0;
    for (uint32_t i = 0; i < this->currentMovement.size(); i = i + 1) {
        if (this->getCurrentAnimationMs() > msLeft) {
            partial = this->currentMovement.at(i);
            break;
        }
        msLeft = msLeft - this->getCurrentAnimationMs();
        if (this->currentMovement.at(i) == toNeg) {
            offset = offset - 1;
        }
        else if (this->currentMovement.at(i) == toPos) {
            offset = offset + 1;
        }
    }
    float partialPart;
    if (partial == toNeg) {
        partialPart = -(float)msLeft / (float)this->getCurrentAnimationMs() * 64;
    }
    else if (partial == toPos) {
        partialPart = (float)msLeft / (float)this->getCurrentAnimationMs() * 64;
    }
    else {
        partialPart = 0;
    }
    return 64 * offset + partialPart;
}
sf::Color Warrior::getTextureColor() const {
    std::vector<sf::Color> colors;
    if (this->inRage()) {
        colors.emplace_back(150, 0, 255);
    }
    if (this->poison) {
        colors.emplace_back(100, 150, 100);
    }
    if (this->inspired) {
        colors.emplace_back(130, 210, 255);
    }
    if (colors.empty()) {
        return this->Unit::getTextureColor();
    }
    return ColorBlender::get().blend(colors);
}
HorizontalSelectionWindowComponent Warrior::getRageModeComponent() const {
    return {
          "rage_spell",
        StringLcl("{rage_spell_description}") + "\n" +
        StringLcl("{moves_left}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->rageModeMovesLeft),
        false,
        Events()
    };
}
HorizontalSelectionWindowComponent Warrior::getPoisonComponent() const {
    return {
            "poison_effect_icon",
            StringLcl("{poison_effect}"),
            false,
            Events()
    };
}
HorizontalSelectionWindowComponent Warrior::getInspiredComponent() const {
    return {
            "inspired_effect_icon",
            StringLcl("{inspired_effect}"),
            false,
            Events()
    };
}
HorizontalSelectionWindowComponent Warrior::getTimeModComponent(MapState *state) const {
    bool isPositive = Player::IS_POSITIVE(this->getPlayerId());
    int32_t d = state->getTimePtr()->getTimeMod().getPercentDelta(isPositive);
    if (d > 0) {
        if (isPositive) {
            return {
                "positive_good_icon",
                StringLcl("{positive_good}"),
                false,
                Events()
            };
        }
        else {
            return {
                "negative_good_icon",
                StringLcl("{negative_good}"),
                false,
                Events()
            };
        }
    }
    else if (d < 0) {
        if (isPositive) {
            return {
                "positive_bad_icon",
                StringLcl("{positive_bad}"),
                false,
                Events()
            };
        }
        else {
            return {
                "negative_bad_icon",
                StringLcl("{negative_bad}"),
                false,
                Events()
            };
        }
    }
    else {
        if (isPositive) {
            return {
                "positive_neutral_icon",
                StringLcl("{positive_neutral}"),
                false,
                Events()
            };
        }
        else {
            return {
                "negative_neutral_icon",
                StringLcl("{negative_neutral}"),
                false,
                Events()
            };
        }
    }
}
HorizontalSelectionWindowComponent Warrior::getKillComponent() {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events killNextTurnEvent;
    killNextTurnEvent.add(std::make_shared<KillNextTurnEvent>(this->getThis<Warrior>()));

    std::shared_ptr<WindowTwoButtons> verifyWindow = std::make_shared<WindowTwoButtons>(StringLcl("{verify_kill}"), StringLcl("{yes}"), StringLcl("{no}"), killNextTurnEvent, clickSoundEvent);

    Events killNextTurnEventVerify = clickSoundEvent;
    killNextTurnEventVerify.add(std::make_shared<ResetHighlightEvent>());
    killNextTurnEventVerify.add(std::make_shared<CreateEEvent>(verifyWindow));

    return {
          "kill_icon",
        StringLcl("{kill}"),
        true,
        killNextTurnEventVerify
    };
}
HorizontalSelectionWindowComponent Warrior::getRevertKillComponent() {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events revertKillNextTurnEvent;
    revertKillNextTurnEvent.add(std::make_shared<RevertKillNextTurnEvent>(this->getThis<Warrior>()));

    return {
          "kill_icon",
        StringLcl("{revert_kill}"),
        true,
        revertKillNextTurnEvent
    };
}
HorizontalSelectionWindowComponent Warrior::getWarriorInfoComponent(MapState *state) const {
    return {
          "helmet_icon",
        StringLcl("{hp}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR(this->getHP(), this->getMaxHP())) + std::to_string(this->getHP()) + " / " + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_BEST) + std::to_string(this->getMaxHP()) + " " + this->getDefence().getReadable() + "\n" +
        this->getSpecialInfoString(state) + "\n" +
        StringLcl("{movement_points}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR(this->movementPoints.value_or(this->getMovementPoints()), this->getMovementPoints())) + std::to_string(this->movementPoints.value_or(this->getMovementPoints())) + " / " + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_BEST) + std::to_string(this->getMovementPoints()) + "\n" +
        StringLcl("{population}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->getPopulation()),
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
Events Warrior::getSelectionWindow(MapState *state, bool own, bool minimal) {
    Events events;

    std::vector<HorizontalSelectionWindowComponent> components;
    components.push_back(this->getExitComponent());
    if (minimal) {
        events.add(std::make_shared<PlaySoundEvent>("click"));
    }
    else {
        if (!own) {
            components.push_back(this->getWarriorOfEnemyComponent());
        }
        components.push_back(this->getDescriptionComponent());
        components.push_back(this->getWarriorInfoComponent(state));
        components.push_back(this->getTimeModComponent(state));
        if (this->inRage()) {
            components.push_back(this->getRageModeComponent());
        }
        if (this->poison) {
            components.push_back(this->getPoisonComponent());
        }
        if (this->inspired) {
            components.push_back(this->getInspiredComponent());
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
    }

    std::shared_ptr<HorizontalSelectionWindow> w = std::make_shared<HorizontalSelectionWindow>(components);
    events.add(std::make_shared<CreateEEvent>(w));

    return events;
}
void Warrior::addHp(uint32_t val) {
    this->Unit::addHp(val);
    this->wasHealedThisMove = true;
}
Events Warrior::getResponse(MapState *state, uint32_t playerId, uint32_t button) {
	if (!this->exist()) {
		return Events();
	}

    if (button == sf::Mouse::Button::Right or (button == sf::Mouse::Button::Left and !this->belongTo(playerId))) {
        return this->getMoveHighlightionEvent(state) + this->getSelectionWindow(state, this->belongTo(playerId), !this->belongTo(playerId) and button == sf::Mouse::Button::Left);
    }

    if (!this->belongTo(playerId)) {
        return Events();
    }

	Events response;

	response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
    if (this->movementPoints.value_or(this->getMovementPoints()) > 0 or this->hasSpecialMoves) {
        Events selectThisEvent = this->getMoveHighlightionEvent(state);
        selectThisEvent.add(std::make_shared<SelectEvent>(this->getThis<Warrior>()));
        selectThisEvent.add(std::make_shared<DisableCursorEvent>());

        response = response + selectThisEvent;
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