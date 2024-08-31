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


class WarriorHealer : public Warrior {
public:
    WarriorHealer();
    WarriorHealer(uint32_t x, uint32_t y, uint32_t playerId);

    uint32_t getHealingSpeed(MapState *state) const;
    bool blockBuildingAbility() const override;
protected:
    virtual bool healVehicles() const;
    virtual uint32_t getBaseHealingSpeed() const = 0;
    virtual Events heal(MapState *state, std::shared_ptr<Warrior> w);
    virtual bool canHeal(std::shared_ptr<Warrior> w) const;
    StringLcl getSpecialInfoString(MapState *state) const override;
private:
    std::string getDirection(std::shared_ptr<Warrior> w) const;
    Events newMove(MapState *state, uint32_t playerId) override;
    std::vector<SpecialMove> getSpecialMoves(MapState *state) const override;
    Events handleSpecialMove(MapState *state, uint32_t targetX, uint32_t targetY) override;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Warrior>(*this);
    }
};


BOOST_CLASS_EXPORT_KEY(WarriorHealer)