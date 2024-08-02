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


#include <algorithm>
#include <limits>
#include "ShootingSpec.hpp"
#include "HighlightColors.hpp"
#include "Locales.hpp"
#include "Building.hpp"
#include "ShootingPriorityComp.hpp"
#include "Damage.hpp"
#include "CreateEEvent.hpp"
#include "FocusOnEvent.hpp"


ShootingSpec::ShootingSpec() = default;
Events ShootingSpec::getActiveNewMoveEvent(const Building *b, MapState *state) {
    if (!b->wasWithFullHP()) {
        return Events();
    }

    std::vector<Unit*> toShoot;
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalUnits(); i = i + 1) {
        Unit* u = state->getCollectionsPtr()->getUnit(i);
        if (u->exist() and u->getPlayerId() != b->getPlayerId() and this->inRadius(b, state, u->getX(), u->getY(), u->getSX(), u->getSY(), AreaControllerSpec::IN_RADIUS_TYPE::PARTIALLY)) {
            toShoot.push_back(u);
        }
    }

    ShootingPriorityComp comp(b);
    std::sort(toShoot.begin(), toShoot.end(), comp);

    std::vector<uint32_t> shots;
    uint32_t shotsLeft = this->getShotsNumber();
    for (uint32_t i = 0; i < toShoot.size(); i = i + 1) {
        Unit *u = toShoot[i];
        uint32_t hpLoss = this->getDamage().getHpLoss(u->getDefence());
        uint32_t shotsToKill;
        if (hpLoss == 0) {
            shotsToKill = std::numeric_limits<uint32_t>::max();
        }
        else {
            shotsToKill = u->getHP() / hpLoss + (bool)(u->getHP() % hpLoss);
        }
        shots.push_back(std::min(shotsLeft, shotsToKill));
        shotsLeft = shotsLeft - shots.back();
        if (shotsLeft == 0) {
            break;
        }
    }

    Events events;

    if (!shots.empty()) {
        events.add(std::make_shared<FocusOnEvent>(b->getX(), b->getY(), b->getSX(), b->getSY()));
    }

    for (uint32_t i = 0; i < shots.size(); i = i + 1) {
        Unit *u = toShoot[i];

        std::shared_ptr<Projectile> projectile = this->getProjectile();
        projectile->setSrc(b->getXInPixels() + b->getSX() * 64 / 2, b->getYInPixels() + b->getSY() * 64 / 2);
        projectile->setDst(u->getXInPixels() + u->getSX() * 64 / 2, u->getYInPixels() + u->getSY() * 64 / 2);

        for (uint32_t j = 0; j < shots[i]; j = j + 1) {
            events.add(std::make_shared<PlaySoundEvent>(projectile->getSoundName()));
            events.add(std::make_shared<CreateEEvent>(projectile));
        }
        events = events + u->hit(shots[i] * this->getDamage());
    }

    return events;
}
std::vector<BuildingHorizontalSelectionWindowComponent> ShootingSpec::getComponents(const Building *b, MapState *state) {
    BuildingHorizontalSelectionWindowComponent component;

    if (b->wasWithFullHP()) {
        component = {HorizontalSelectionWindowComponent(this->getProjectile()->getIconTextureName(),
                                                       *Locales::get()->get("this_building_shoots_to_enemies") + this->getDamage().getReadable() + L" x " + std::to_wstring(this->getShotsNumber()),
                                                       false,
                                                       Events()),
                true};
    }
    else {
        component = {HorizontalSelectionWindowComponent("hammer_icon",
                                                        *Locales::get()->get("does_not_shoot_if_isnt_built_yet"),
                                                        false,
                                                        Events()),
                     true};
    }

    return {component};
}
uint32_t ShootingSpec::getRadius() const {
    return this->getShootingRadius();
}
sf::Color ShootingSpec::getHighlightColor(uint32_t playerId) const {
    return HighlightColors::get()->getBuildingAttackColor(playerId);
}
uint8_t ShootingSpec::getHighlightType() const {
    return AreaControllerSpec::HIGHLIGHT_TYPE::ATTACK;
}
bool ShootingSpec::ignoreLowObstacles() const {
    return true;
}
bool ShootingSpec::ignoreHighObstacles() const {
    return true;
}
uint8_t ShootingSpec::getNewMoveMainPriority() const {
    return 2;
}


BOOST_CLASS_EXPORT_IMPLEMENT(ShootingSpec)