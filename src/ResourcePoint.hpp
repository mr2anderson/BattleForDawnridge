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


class ResourcePoint : public HPGO {
public:
	ResourcePoint();
	ResourcePoint(uint32_t x, uint32_t y);

	Events newMove(MapState *state, uint32_t playerId) override;
	virtual uint32_t tryToCollect(uint32_t playerId, uint32_t value);
	virtual std::string getResourceType() const = 0;
	bool warriorCanStay(std::shared_ptr<const Warrior> w) const override;
	uint32_t getWarriorMovementCost(std::shared_ptr<Warrior> w) const override;
    StringLcl getDetailedDescription(MapState *state) const override;
protected:
    virtual bool isArea() const = 0;
    virtual bool isConduction() const = 0;
private:
	Events getResponse(MapState *state, uint32_t playerId, uint32_t button) override;
	Events getSelectionWindow();
	HorizontalSelectionWindowComponent getResourceLeftComponent() const;
	HorizontalSelectionWindowComponent getSlowMovementComponent() const;
	uint32_t getWalkingWarriorMovementCost() const;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<HPGO>(*this);
    }
};


BOOST_CLASS_EXPORT_KEY(ResourcePoint)