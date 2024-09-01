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
#include "IShootingSpec.hpp"
#include "HighlightColors.hpp"
#include "Locales.hpp"
#include "Building.hpp"
#include "ShootingPriorityComp.hpp"
#include "Damage.hpp"
#include "CreateEEvent.hpp"
#include "FocusOnEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "ColorTheme.hpp"


Events IShootingSpec::getActiveNewMoveEvent(std::shared_ptr<const Building> b, MapState *state) {
    if (!b->wasWithFullHP()) {
        return Events();
    }

    HashTableMapPosition<uint32_t> available = this->getAvailable(b->getX(), b->getY(), b->getSX(), b->getSY(), b->getPlayerId(), state);
    std::vector<std::shared_ptr<Unit>> toShoot;
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalUnits(); i = i + 1) {
        std::shared_ptr<Unit> u = state->getCollectionsPtr()->getUnit(i);
        if (u->exist() and u->getPlayerId() != b->getPlayerId() and IAreaControllerSpec::IN_RADIUS(available, u, IAreaControllerSpec::IN_RADIUS_TYPE::PARTIALLY)) {
            toShoot.push_back(u);
        }
    }

    ShootingPriorityComp comp(b);
    std::sort(toShoot.begin(), toShoot.end(), comp);

    std::vector<uint32_t> shots;
    uint32_t shotsLeft = this->getShotsNumber();
    for (uint32_t i = 0; i < toShoot.size(); i = i + 1) {
        std::shared_ptr<Unit>u = toShoot[i];
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
        std::shared_ptr<Unit>u = toShoot[i];

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
std::vector<BuildingHorizontalSelectionWindowComponent> IShootingSpec::getComponents(std::shared_ptr<const Building> b, MapState *state) {
    BuildingHorizontalSelectionWindowComponent component;

    if (b->wasWithFullHP()) {
        component = {HorizontalSelectionWindowComponent(this->getProjectile()->getTextureName(),
                                                       StringLcl("{this_building_shoots_to_enemies}") + this->getDamage().getReadable() + " x " + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->getShotsNumber()),
                                                       false,
                                                       Events()),
                true};
    }
    else {
        component = {HorizontalSelectionWindowComponent(  "hammer_icon",
                                                        StringLcl("{does_not_shoot_if_isnt_built_yet}"),
                                                        false,
                                                        Events()),
                     true};
    }

    return {component};
}
uint32_t IShootingSpec::getRadius() const {
    return this->getShootingRadius();
}
sf::Color IShootingSpec::getHighlightColor(uint32_t playerId) const {
    return HighlightColors::get().getBuildingAttackColor(playerId);
}
uint8_t IShootingSpec::getHighlightType() const {
    return IAreaControllerSpec::HIGHLIGHT_TYPE::OTHER;
}
bool IShootingSpec::ignoreLowObstacles() const {
    return true;
}
bool IShootingSpec::ignoreHighObstacles() const {
    return true;
}
NewMoveMainPriority IShootingSpec::getNewMoveMainPriority() const {
    return GO::NEW_MOVE_MAIN_PRIORITY_SHOOTING;
}


BOOST_CLASS_EXPORT_IMPLEMENT(IShootingSpec)