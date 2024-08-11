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


#include <boost/serialization/access.hpp>
#include <boost/serialization/set.hpp>
#include "Map.hpp"
#include "PopUpElement.hpp"
#include "HighlightTable.hpp"
#include "Button.hpp"


#pragma once


class WorldUIState {
public:
	WorldUIState(Map *mapPtr, std::shared_ptr<PopUpElement> *element, HighlightTable *table, bool *cursorVisibility);
    WorldUIState(const WorldUIState& state) = delete;

    enum FLAG {
        SAVE,
        LOAD
    };
private:
    Map *map;
    std::shared_ptr<PopUpElement> *element;
    HighlightTable *highlightTable;
    bool *curcorVisibility;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar & this->map;
        ar & this->element;
        ar & this->highlightTable;
        ar & this->curcorVisibility;
    }
};