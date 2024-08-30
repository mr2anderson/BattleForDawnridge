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
#include "AddHpEvent.hpp"
#include "PlaySoundEvent.hpp"


WarriorProjectileHealer::WarriorProjectileHealer() = default;
WarriorProjectileHealer::WarriorProjectileHealer(uint32_t x, uint32_t y, uint32_t playerId) : WarriorHealer(x, y, playerId) {

}
Events WarriorProjectileHealer::heal(MapState *state, std::shared_ptr<Warrior> w) {
    Events events = this->WarriorHealer::heal(state, w);

    std::shared_ptr<Projectile> projectile = this->getProjectile();
    events.add(std::make_shared<PlaySoundEvent>(projectile->getSoundName()));
    projectile->setSrc(this->getXInPixels() + 64 / 2, this->getYInPixels() + 64 / 2);
    projectile->setDst(w->getXInPixels() + 64 / 2, w->getYInPixels() + 64 / 2);
    events.add(std::make_shared<CreateEEvent>(projectile));

    events.add(std::make_shared<PlaySoundEvent>("heal"));
    events.add(std::make_shared<CreateEEvent>(std::make_shared<HPFlyingE>(std::min(this->getHealingSpeed(state), w->getMaxHP()- w->getHP()), true, w->getX(), w->getY(), w->getSX(), w->getSY())));
    events.add(std::make_shared<AddHpEvent>(w, this->getHealingSpeed(state)));

    return events;
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarriorProjectileHealer)