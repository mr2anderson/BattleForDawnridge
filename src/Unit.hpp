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
#include "Resources.hpp"
#include "PlayerPointer.hpp"
#include "Defence.hpp"
#include "Damage.hpp"


#pragma once


class Unit : public HPGO {
public:
	Unit();
	Unit(uint32_t x, uint32_t y, boost::optional<uint32_t> currentHp, uint32_t playerId);

	bool hasError(MapSize mapSize, uint32_t totalPlayers) const override {
		return this->HPGO::hasError(mapSize, totalPlayers) or this->playerId == 0 or this->playerId > totalPlayers;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Events hit(Damage d);
	virtual Events hit(uint32_t d) = 0;
	bool belongTo(uint32_t playerId) const;
	void changePlayer(uint32_t newPlayerId);
	uint32_t getPlayerId() const;

	virtual Defence getDefence() const = 0;
	virtual Resources getCost() const = 0;
    StringLcl getDetailedDescription(MapState *state) const override;
protected:
    virtual std::shared_ptr<PlayerPointer> getPlayerPointer() const = 0;
	std::shared_ptr<ILightSource> getLightSource() const override;
private:
	uint32_t playerId;

	void drawPlayerPointer(sf::RenderTarget& target, sf::RenderStates states) const;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<HPGO>(*this);
        ar & this->playerId;
    }
};


BOOST_CLASS_EXPORT_KEY(Unit)