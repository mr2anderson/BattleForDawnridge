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


#include "WarriorHealer.hpp"
#include "Parameters.hpp"
#include "Locales.hpp"
#include "HighlightColors.hpp"
#include "ChangeWarriorDirectionEvent.hpp"
#include "FocusOnEvent.hpp"
#include "ColorTheme.hpp"


WarriorHealer::WarriorHealer() = default;
WarriorHealer::WarriorHealer(uint32_t x, uint32_t y, uint32_t playerId) : Warrior(x, y, playerId) {

}
bool WarriorHealer::blockBuildingAbility() const {
    return false;
}
Events WarriorHealer::heal(MapState *state, std::shared_ptr<Warrior> w) {
    Events changeWarriorDirectionEvent;
    changeWarriorDirectionEvent.add(std::make_shared<ChangeWarriorDirectionEvent>(w, this->getDirection(w)));
    return changeWarriorDirectionEvent;
}
std::string WarriorHealer::getDirection(std::shared_ptr<Warrior> w) const {
    if (w->getY() > this->getY()) {
        return "s";
    }
    if (w->getY() < this->getY()) {
        return "n";
    }
    if (w->getX() > this->getX()) {
        return "e";
    }
    return "w";
}
Events WarriorHealer::newMove(MapState *state, uint32_t playerId) {
    Events events = Warrior::newMove(state, playerId);

    if (this->exist() and playerId == this->getPlayerId()) {
        bool focusAdded = false;
        for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
            std::shared_ptr<Warrior> w = state->getCollectionsPtr()->getWarrior(i);
            if (this->canHeal(w)) {
                if (!focusAdded) {
                    events.add(std::make_shared<FocusOnEvent>(this->getX(), this->getY(), this->getSX(), this->getSY()));
                    focusAdded = true;
                }
                events = events + this->heal(state, w);
            }
        }
    }

    return events;
}
bool WarriorHealer::healVehicles() const {
    return false;
}
bool WarriorHealer::canHeal(std::shared_ptr<Warrior> w) const {
    if (!w->exist() or w->getPlayerId() != this->getPlayerId() or w->getHP() == w->getMaxHP() or w->isVehicle() != this->healVehicles() or w->wasHealed() or w->getUUID() == this->getUUID()) {
        return false;
    }
    return true;
}
std::vector<SpecialMove> WarriorHealer::getSpecialMoves(MapState *state) const {
    std::vector<SpecialMove> moves;
    return moves;
}
Events WarriorHealer::handleSpecialMove(MapState *state, uint32_t targetX, uint32_t targetY) {
    return Events();
}
StringLcl WarriorHealer::getSpecialInfoString(MapState *state) const {
    StringLcl str = StringLcl("{healing_speed}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(Parameters::get().getInt("healing_speed")) + ".";

    return str;
}
NewMoveMainPriority WarriorHealer::getNewMoveMainPriority() const {
    return NEW_MOVE_MAIN_PRIORITY_WARRIOR_HEALER;
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarriorHealer)