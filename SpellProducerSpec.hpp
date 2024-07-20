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

	Events getActiveNewMoveEvent(const Building* building, MapState* state) override;
	std::vector<HorizontalSelectionWindowComponent> getComponents(const Building* building, MapState* state) override;
	std::optional<BuildingShortInfo> getShortInfo(const Building* building) const override;
	void setSpell(std::shared_ptr<Spell> newSpell);
	virtual std::vector<std::shared_ptr<Spell>> getSpellsToProduce(uint32_t playerId) const = 0;
private:
	Spell* spell;
};