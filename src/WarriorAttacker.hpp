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


#pragma once


class WarriorAttacker : public Warrior {
public:
    WarriorAttacker();
    WarriorAttacker(uint32_t x, uint32_t y, uint32_t playerId);

    bool hasError(MapSize mapSize, uint32_t totalPlayers) const override {
        return 
            (this->Warrior::hasError(mapSize, totalPlayers) or 
            (this->getCurrentAnimation() == "attack" and (this->target == nullptr or this->target->hasError(mapSize, totalPlayers))));
    }

    Damage getDamage() const;
    uint32_t getAnimationNumber(const std::string& type, const std::string& direction) const override;
    bool blockBuildingAbility() const override;
    virtual uint32_t getAttackAnimationsNumberInSet() const = 0;
    virtual std::string getStartAttackSoundName() const = 0;
protected:
    virtual std::vector<std::string> getAttackPossibleDirections() const = 0;
    virtual std::vector<std::tuple<uint32_t, uint32_t>> canAttack(Unit *u) const;
    virtual Events startAttack(Unit *u, uint32_t targetX, uint32_t targetY);
    virtual Damage getBaseDamage() const = 0;
private:
    Unit *target;

    uint32_t getCurrentAnimationMs() const override;
    std::vector<SpecialMove> getSpecialMoves(MapState *state) const override;
    std::string getDirection(uint32_t targetX, uint32_t targetY) const;
    Events handleSpecialMove(MapState *state, uint32_t targetX, uint32_t targetY) override;
    Events processCurrentAnimation(MapState *state) override;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Warrior>(*this);
        ar & this->target;
    }
};


BOOST_CLASS_EXPORT_KEY(WarriorAttacker)