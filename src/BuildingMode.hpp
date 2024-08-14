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



#include <boost/serialization/shared_ptr.hpp>
#include "ISelectable.hpp"


#pragma once


class BuildingMode : public ISelectable {
public:
    BuildingMode();
	BuildingMode(std::shared_ptr<const Building> b, uint32_t playerId);

	Events start(MapState *state);
private:
	std::shared_ptr<const Building> b;
	uint32_t playerId;

	std::shared_ptr<sf::Drawable> getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const override;
	Events unselect(MapState *state, uint32_t x, uint32_t y, uint8_t button) override;

	Events getHighlightEvent(MapState *state) const;

	bool inMap(MapState* state, const Building *clonedB) const;
	bool empty(MapState* state, const Building *clonedB) const;
	bool controlled(MapState* state, const Building *clonedB) const;
    bool noEnemyWarriors(MapState* state, const Building *clonedB) const;

	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive& ar, const unsigned int version) {
		ar& this->b;
		ar& this->playerId;
	}
};


BOOST_CLASS_EXPORT_KEY(BuildingMode)