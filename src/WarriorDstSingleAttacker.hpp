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


#include "WarriorDstAttacker.hpp"
#include "ISingleAttacker.hpp"


#pragma once


class WarriorDstSingleAttacker : public WarriorDstAttacker, public ISingleAttacker {
public:
    WarriorDstSingleAttacker();
    WarriorDstSingleAttacker(uint32_t x, uint32_t y, uint32_t playerId);
private:
    bool attackAbility;

    Events endMove(MapState *state, uint32_t currentPlayerId) override;
    std::vector<std::tuple<uint32_t, uint32_t>> canAttack(std::shared_ptr<Unit>u) const override;
    Events startAttack(MapState *state, std::shared_ptr<Unit>u, uint32_t targetX, uint32_t targetY) override;
    StringLcl getSpecialInfoString(MapState *state) const override;
    void refreshAbility() override;
    void wipeAbility() override;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<WarriorDstAttacker>(*this);
        ar & this->attackAbility;
    }
};


BOOST_CLASS_EXPORT_KEY(WarriorDstSingleAttacker)