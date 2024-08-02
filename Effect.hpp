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



#include "HPGO.hpp"


#pragma once


class Effect : public HPGO {
public:
	Effect();
	Effect(uint32_t x, uint32_t y, uint32_t playerId);

	uint32_t getSX() const override;
	uint32_t getSY() const override;
    uint32_t getPlayerId() const;
    uint8_t getClickPriority() const override;
    uint8_t getDrawingPriority() const override;
    uint8_t getNewMoveMainPriority() const override;
protected:
	virtual Events getActiveNewMoveEvent(MapState* state, uint32_t currentPlayerId) const = 0;
	virtual uint32_t getLifetime() const = 0;
private:
	uint32_t playerId;

	uint32_t getMaxHP() const override;
	Events newMove(MapState* state, uint32_t playerId) override;
	Events getResponse(MapState* state, uint32_t playerId, uint32_t button) override;
	HorizontalSelectionWindowComponent getEffectComponent() const;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<HPGO>(*this);
        ar & this->playerId;
    }
};


BOOST_CLASS_EXPORT_KEY(Effect)