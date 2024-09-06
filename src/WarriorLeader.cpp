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


#include "WarriorLeader.hpp"
#include "Parameters.hpp"
#include "CreateAnimationEvent.hpp"
#include "CloseAnimationEvent.hpp"
#include "HighlightColors.hpp"
#include "PlaySoundEvent.hpp"
#include "GlobalRandomGenerator32.hpp"
#include "StartWarriorAnimationEvent.hpp"
#include "ChangeWarriorDirectionEvent.hpp"
#include "FocusOnEvent.hpp"
#include "ColorTheme.hpp"


WarriorLeader::WarriorLeader() = default;
WarriorLeader::WarriorLeader(uint32_t x, uint32_t y, uint32_t playerId) : Warrior(x, y, playerId) {

}
Events WarriorLeader::newMove(MapState *state, uint32_t playerId) {
    Events events = Warrior::newMove(state, playerId);

    if (this->exist() and playerId == this->getPlayerId()) {
        bool focusAdded = false;
        for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
            std::shared_ptr<Warrior> w = state->getCollectionsPtr()->getWarrior(i);
            if (this->canInspire(w)) {
                if (!focusAdded) {
                    events.add(std::make_shared<FocusOnEvent>(this->getX(), this->getY(), this->getSX(), this->getSY()));
                    focusAdded = true;
                }
                events = events + this->startInspire(state, w);
            }
        }
    }

    return events;
}
bool WarriorLeader::blockBuildingAbility() const {
    return false;
}
std::vector<SpecialMove> WarriorLeader::getSpecialMoves(MapState *state) const {
    return {};
}
bool WarriorLeader::canInspire(std::shared_ptr<Warrior> w) const {
    if (!w->exist() or w->getPlayerId() != this->getPlayerId() or w->isInspired() or this->getUUID() == w->getUUID()) {
        return false;
    }
    uint32_t dx = std::max(this->getX(), w->getX()) - std::min(this->getX(), w->getX());
    uint32_t dy = std::max(this->getY(), w->getY()) - std::min(this->getY(), w->getY());
    return (dx <= this->getInspireRadius() and dy <= this->getInspireRadius());
}
Events WarriorLeader::handleSpecialMove(MapState *state, uint32_t targetX, uint32_t targetY) {
    return Events();
}
Events WarriorLeader::startInspire(MapState *state, std::shared_ptr<Warrior> w) {
    Events events;

    this->wipeMovementPoints();

    events.add(std::make_shared<StartWarriorAnimationEvent>(this->getThis<Warrior>(), "inspire"));
    events.add(std::make_shared<ChangeWarriorDirectionEvent>(this->getThis<Warrior>(), this->getDirectionTo(w->getX(), w->getY())));

    events.add(std::make_shared<PlaySoundEvent>(this->getStartInspireSoundName()));

    events.add(std::make_shared<CreateAnimationEvent>(SuspendingAnimation(this->getThis<WarriorLeader>())));

    events = events + w->inspire();

    return events;
}
boost::optional<SpecialAnimation> WarriorLeader::getSpecialAnimation() const {
    SpecialAnimation special;
    special.name = "inspire";
    special.ms = 600;
    special.straightFrames = this->getInspireAnimationsNumberInSet();
    special.obliquelyFrames = this->getInspireAnimationsNumberInSet();
    return special;
}
Events WarriorLeader::processSpecialAnimation() {
    Events events;

    if (this->getCurrentAnimationState().finished) {
        events.add(std::make_shared<CloseAnimationEvent>());
        events.add(std::make_shared<StartWarriorAnimationEvent>(this->getThis<Warrior>(), "talking"));
    }

    return events;
}
StringLcl WarriorLeader::getSpecialInfoString(MapState *state) const {
    return StringLcl("{leader}. {inspiration_radius}: ") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + StringLcl(std::to_string(this->getInspireRadius()));
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarriorLeader)