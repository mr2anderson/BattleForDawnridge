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


#include "WarriorProjectileHealer.hpp"
#include "CreateEEvent.hpp"
#include "HPFlyingE.hpp"
#include "PlaySoundEvent.hpp"
#include "ColorTheme.hpp"


WarriorProjectileHealer::WarriorProjectileHealer() = default;
WarriorProjectileHealer::WarriorProjectileHealer(uint32_t x, uint32_t y, uint32_t playerId) : WarriorHealer(x, y, playerId) {

}
bool WarriorProjectileHealer::canHeal(std::shared_ptr<Warrior> w) const {
    if (!WarriorHealer::canHeal(w)) {
        return false;
    }
    uint32_t dx = std::max(this->getX(), w->getX()) - std::min(this->getX(), w->getX());
    uint32_t dy = std::max(this->getY(), w->getY()) - std::min(this->getY(), w->getY());
    return (dx <= this->getHealingRadius() and dy <= this->getHealingRadius());
}
Events WarriorProjectileHealer::heal(MapState *state, std::shared_ptr<Warrior> w) {
    Events events = this->WarriorHealer::heal(state, w);

    std::shared_ptr<Projectile> projectile = this->getProjectile();
    events.add(std::make_shared<PlaySoundEvent>(projectile->getSoundName()));
    projectile->setSrc(this->getXInPixels() + 64 / 2, this->getYInPixels() + 64 / 2);
    projectile->setDst(w->getXInPixels() + 64 / 2, w->getYInPixels() + 64 / 2);
    events.add(std::make_shared<CreateEEvent>(projectile));

    Events healEvent = w->heal();
    events = events + healEvent;

    return events;
}
StringLcl WarriorProjectileHealer::getSpecialInfoString(MapState *state) const {
    StringLcl str = this->WarriorHealer::getSpecialInfoString(state) + " " + StringLcl("{healing_radius}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->getHealingRadius()) + ". ";

    return str;
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarriorProjectileHealer)