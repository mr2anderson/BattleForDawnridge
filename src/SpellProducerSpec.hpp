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


#include "IBuildingSpec.hpp"
#include "Spell.hpp"


#pragma once


class SpellProducerSpec : public IBuildingSpec {
public:
	SpellProducerSpec();
	~SpellProducerSpec() override;

	bool hasError(MapSize mapSize, uint32_t totalPlayers) const override {
		if (this->IBuildingSpec::hasError(mapSize, totalPlayers)) {
			return true;
		}
		if (this->spell != nullptr) { // Spell ptr can be null here
			return this->spell->hasError(mapSize, totalPlayers);
		}
		return false;
	}

	Events getActiveNewMoveEvent(const Building* building, MapState* state) override;
	std::vector<BuildingHorizontalSelectionWindowComponent> getComponents(const Building* building, MapState* state) override;
	boost::optional<BuildingShortInfo> getShortInfo(const Building* building) const override;
	void setSpell(std::shared_ptr<Spell> newSpell);
	virtual std::vector<std::shared_ptr<Spell>> getSpellsToProduce(uint32_t playerId) const = 0;
private:
	Spell* spell;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<IBuildingSpec>(*this);
        ar & this->spell;
    }
};


BOOST_CLASS_EXPORT_KEY(SpellProducerSpec)